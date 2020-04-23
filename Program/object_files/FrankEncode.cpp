#include "../header_files/FrankEncode.h"
#include "../header_files/FileToEncode.h"
#include "../header_files/ImageToOutput.h"
#include "../header_files/CoverImage.h"
#include "../header_files/CoverPixel.h"
#include "../header_files/sha512.h"
#include "../header_files/Converter.h"
#include <Magick++.h>
#include <iostream>
#include <string>
#include <vector>
#include <sodium.h>
#include <math.h>

using namespace std;

/**
Creates the FrankEncode object, this sets the program up read for encoding.
It will create the ouput file, read the input file and do error checking.
This will exit if there are any errors.
@param char **argv Array of parameters from command line.
@pramr bool isImage would the user like the output to be an image?
*/
FrankEncode::FrankEncode(char **argv, bool isImage){
	cout << "[INFO]: Opening cover image." << endl;
    image = CoverImage(argv[2]);

	outputImage = isImage;

	cout << "[INFO]: Opening file to encode." << endl;
	plainFile = FileToEncode(argv[3]);

    char myString[32];
    randombytes_buf(myString, 32);

    if(!image.isValid()){
        cout << "Could not read cover file, closing program." << endl;
        exit(3);
    }

    if(image.getMaxPixels() < 1000){
        cout << "[ERROR] The image you have chosen is too small." << endl;
        cout << "You need at least 1000 pixels in an image." << endl;
        exit(4);
    }

	if(!plainFile.isValid()){
		cout << "[ERROR]: Program exiting" << endl;
		exit(6);
	}

	if(outputImage){
		cout << "[INFO]: Creating output image." << endl;
		size_t recommendedSize = calculateBestImageSize(plainFile.getTotalSize());
		outputFileImage = ImageToOutput(argv[4], recommendedSize, recommendedSize);

		if(!outputFileImage.isValid()){
			cout << "[ERROR]: Program exiting" << endl;
			exit(7);
		}
	}

	else{
		cout << "[INFO]: Creating output file." << endl;
		outputFileFile = FileToOutput(argv[4]);

		if(!outputFileFile.isValid()){
			cout << "[ERROR]: Program exiting" << endl;
			exit(7);
		}
	}

	pixelsUsed = new bool*[image.getWidth()];

	for(int i = 0; i < image.getWidth(); i++){
		pixelsUsed[i] = new bool[image.getHeight()];
	}

	getPixels(1000);
}

/**
Finds the next available pixel in the cover image and returns it to the method that called it.
@return the next available pixel in an image as a CoverPixel object.
*/
CoverPixel FrankEncode::findPixel(){
    bool invalid = true;
    do{
        size_t * loc = image.getNextLocation();
		size_t ix = loc[0];
		size_t iy = loc[1];

		delete [] loc;

        if(!pixelsUsed[ix][iy]){
            invalid = false;
			pixelsUsed[ix][iy] = true;
            string colour = image.getHexColour(ix, iy);
            image.claimUsedPixel();
			image.resetFailedAttempts();
            return CoverPixel(ix, iy, colour);
        }

    } while(invalid);
	return CoverPixel();
}

/**
Finds the specified amount of unique pixels and adds them to a vector holding
all in use pixels.
@param int pixelAmount: Amount of pixels you would like.
@return true if added all the requested pixels, false if not enough
pixels left for the requested amount.
*/
bool FrankEncode::getPixels(int pixelAmount){
    if(image.getPixelsLeft() < pixelAmount)
        return false;

    for(int i = 0; i < pixelAmount; i++){
        pixels.push_back(findPixel());
    }

    return true;
}

/**
Replaces the pixel at the specified location, this will either remove it if there
are no more unique ones left, or replace it with the next unqiue pixel.
@param int location: The location in the vector of the pixel you would like to replace.
*/
void FrankEncode::replacePixel(int location){
    if(pixels.size() <= 1){
        cout << "[ERROR]: Ran out of pixels, please try with a bigger image!" << endl;
        exit(5);
    }

    if(image.getPixelsLeft() < 1){
        cout << "[INFO]: You are running low on pixels!" << endl;
        pixels.erase(pixels.begin() + location);
        pixels.shrink_to_fit();
        return;
    }

    pixels[location] = findPixel();
}

/**
Checks the count of the hex character in the specified pixel location.
@param int pixelLoc: The location in the vector array to check.
@param string letter: The hex character you would like to check.
@return true if there is enough space for that letter or false if the pixel
needs to be replaced.
*/
bool FrankEncode::checkCount(int pixelLoc, string letter){
    size_t amount = pixels[pixelLoc].getLetterCount(letter);
    if(amount < 1){
        replacePixel(pixelLoc);
        return false;
    }
    return true;
}

/**
Encode the letter with a random pixel and location.
@param string hashLetter: The hex letter you would like to encode.
@return Location for the encoded result.
*/
Location FrankEncode::encodeLetter(string hashLetter){

    bool valid;
    int pixelToUse;
    CoverPixel pixel;

    do{
        pixelToUse = randombytes_uniform((uint32_t) pixels.size());
        valid = checkCount(pixelToUse, hashLetter);
    } while(!valid);

    pixel = pixels[pixelToUse];

    size_t x = pixel.getX();
    size_t y = pixel.getY();
    int hashLocation = pixel.getLetter(hashLetter);

    pixels[pixelToUse] = pixel;

    return Location{x, y, hashLocation};
}

/**
Calculate the best image size to use as an output image. This assumes you are
creating a square output image. So it returns only width as height = width.
@param size_t fileSize: The input filesize in bytes.
@return size_t The width of the best image size to use.
*/
size_t FrankEncode::calculateBestImageSize(size_t fileSize){
	size_t withHex = fileSize * 2;
	size_t withPixels = withHex * 3;
	return ((size_t) ceil(sqrt(withPixels)));
}

/**
Method incharge of writing the output image, once called it will loop through the input
and create the output image.
*/
void FrankEncode::writeImage(){
	bool moreToRead = true;

	do{
		char* fileBytes = plainFile.getNextBytes();

		size_t bytesRead = plainFile.getBufferSize();
		string hex = Converter::char2hex(fileBytes, bytesRead);

		for(int i = 0; i < hex.length(); i++){
			string letter = hex.substr(i, 1);
			Location loc = encodeLetter(letter);

			double* xRGB = Converter::hex2rgb(Converter::int2hex((int) loc.x));
			double* yRGB = Converter::hex2rgb(Converter::int2hex((int) loc.y));
			double* hashRGB = Converter::hex2rgb(Converter::int2hex(loc.hash));

			Magick::Color x = Magick::ColorRGB(xRGB[0], xRGB[1], xRGB[2]);
			Magick::Color y = Magick::ColorRGB(yRGB[0], yRGB[1], yRGB[2]);
			Magick::Color hash = Magick::ColorRGB(hashRGB[0], hashRGB[1], hashRGB[2]);

			delete [] xRGB;
			delete [] yRGB;
			delete [] hashRGB;

			outputFileImage.updatePixel(x);
			outputFileImage.updatePixel(y);
			outputFileImage.updatePixel(hash);
		}

		moreToRead = !(plainFile.isFileRead());

		counter++;

		if(counter == 10000){
			counter = 0;
			cout << "Read So Far: " << plainFile.getReadSoFar() << endl;
			cout << "Flushing output file made so far" << endl;
		 	outputFileImage.write();
			cout << "Output file flushed, continuing." << endl;
		}

	} while(moreToRead);

	cout << "[INFO]: File read." << endl;

	outputFileImage.updatePixel(Magick::ColorRGB(255, 255, 255));

	while(!outputFileImage.isWritten()){
		//Needs to be 16777215 so that it cannot generate #FFFFFF (used to determine stopping pixel).
		int random = randombytes_uniform(16777215);
		double* rgb = Converter::hex2rgb(Converter::int2hex(random));

		Magick::Color color = Magick::ColorRGB(rgb[0], rgb[1], rgb[2]);

		delete [] rgb;
		outputFileImage.updatePixel(color);
	}

	outputFileImage.write();
}

/**
Method incharge of writing the output file, once called it will loop through the input
and create the output file.
*/
void FrankEncode::writeFile(){
	bool moreToRead = true;

	do{
		char* fileBytes = plainFile.getNextBytes();

		size_t bytesRead = plainFile.getBufferSize();
		string hex = Converter::char2hex(fileBytes, bytesRead);

		for(int i = 0; i < hex.length(); i++){
			string letter = hex.substr(i, 1);
			Location loc = encodeLetter(letter);

			unsigned short x = (unsigned short) loc.x;
			unsigned short y = (unsigned short)  loc.y;
			unsigned short hash = (unsigned short) loc.hash;

			outputFileFile.write(x);
			outputFileFile.write(y);
			outputFileFile.write(hash);
		}

		moreToRead = !(plainFile.isFileRead());

		counter++;

		if(counter == 10000){
			counter = 0;
			cout << "Read So Far: " << plainFile.getReadSoFar() << endl;
			cout << "Flushing output file made so far" << endl;
			outputFileFile.flush();
			cout << "Output file flushed, continuing." << endl;
		}

	} while(moreToRead);

	cout << "[INFO]: File read." << endl;
}

/**
Method incharge of encoding, once called it will call the relevant output method, will return
once encoding is complete. This may take a while!
*/
void FrankEncode::encode(){

	cout << "[INFO]: Encoding file." << endl;

	if(outputImage){
		writeImage();
	}
	else{
		writeFile();
	}
}

/**
Call this before destorying the object, it will close and destory all relevant children objects.
*/
void FrankEncode::close(){
	for(int i = 0; i < image.getWidth(); i++){
		delete [] pixelsUsed[i];
	}

	delete [] pixelsUsed;

	image.close();
	plainFile.close();

	if(!outputImage){
		outputFileFile.close();
	}
}
