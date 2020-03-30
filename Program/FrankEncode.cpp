#include "FrankEncode.h"
#include "FileToEncode.h"
#include "ImageToOutput.h"
#include "CoverImage.h"
#include "CoverPixel.h"
#include "sha512.h"
#include "Converter.h"
#include <Magick++.h>
#include <iostream>
#include <string>
#include <vector>
#include <sodium.h>
#include <math.h>

using namespace std;

struct compare{
	string key;
	compare(string const &i): key(i) { }

	bool operator()(string const &i)
	{
		return (i == key);
	}
};

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

	getPixels(1000);
}

CoverPixel FrankEncode::findPixel(){
    bool invalid = true;
    do{
        int * loc = image.getNextLocation();
		int ix = loc[0];
		int iy = loc[1];

		delete [] loc;
        string x,y,key;
        x = to_string(ix);
        y = to_string(iy);
        key = sha512(x + "-" + y);

        pair<std::set<string>::iterator,bool> result = pixelsUsed.insert(key);

        if(result.second){
            invalid = false;
            string colour = image.getHexColour(ix, iy);
            image.claimUsedPixel();
            return CoverPixel(ix, iy, colour);
        }

    } while(invalid);

	image.resetFailedAttempts();
}

bool FrankEncode::getPixels(int pixelAmount){
    if(image.getPixelsLeft() < pixelAmount)
        return false;

    for(int i = 0; i < pixelAmount; i++){
        pixels.push_back(findPixel());
    }

    return true;
}

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

bool FrankEncode::checkCount(int pixelLoc, string letter){
    int amount = pixels[pixelLoc].getLetterCount(letter);
    if(amount < 1){
        replacePixel(pixelLoc);
        return false;
    }
    return true;
}

Location FrankEncode::encodeLetter(string hashLetter){

    bool valid;
    int pixelToUse;
    CoverPixel pixel;

    do{
        pixelToUse = randombytes_uniform(pixels.size());
        valid = checkCount(pixelToUse, hashLetter);
    } while(!valid);

    pixel = pixels[pixelToUse];

    int x = pixel.getX();
    int y = pixel.getY();
    int hashLocation = pixel.getLetter(hashLetter);

	// cout << "Pixel X: " << x << endl;
	// cout << "Pixel Y: " << y << endl;
	// cout << "Pixel Hash: " << hashLocation << endl;

    pixels[pixelToUse] = pixel;

    return Location{x, y, hashLocation};
}

size_t FrankEncode::calculateBestImageSize(size_t fileSize){
	size_t withHex = fileSize * 2;
	size_t withPixels = withHex * 3;
	return ceil(sqrt(withPixels));
}

void FrankEncode::writeImage(){
	bool moreToRead = true;

	do{
		char* fileBytes = plainFile.getNextBytes();

		size_t bytesRead = plainFile.getBufferSize();
		string hex = Converter::char2hex(fileBytes, bytesRead);

		for(int i = 0; i < hex.length(); i++){
			string letter = hex.substr(i, 1);
			Location loc = encodeLetter(letter);

			double* xRGB = Converter::hex2rgb(Converter::int2hex(loc.x));
			double* yRGB = Converter::hex2rgb(Converter::int2hex(loc.y));
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

	cout << "File read." << endl;

	outputFileImage.updatePixel(Magick::ColorRGB(255, 255, 255));

	while(!outputFileImage.isWritten()){
		int random = randombytes_uniform(16777216);
		double* rgb = Converter::hex2rgb(Converter::int2hex(random));

		Magick::Color color = Magick::ColorRGB(rgb[0], rgb[1], rgb[2]);

		delete [] rgb;
		outputFileImage.updatePixel(color);
	}

	outputFileImage.write();
}

void FrankEncode::writeFile(){
	bool moreToRead = true;

	do{
		char* fileBytes = plainFile.getNextBytes();

		size_t bytesRead = plainFile.getBufferSize();
		string hex = Converter::char2hex(fileBytes, bytesRead);

		for(int i = 0; i < hex.length(); i++){
			string letter = hex.substr(i, 1);
			Location loc = encodeLetter(letter);

			// cout << "X: " << loc.x << endl;
			// cout << "Y: " << loc.y << endl;
			// cout << "Hash: " << loc.hash << endl;

			ushort x = (ushort) loc.x;
			ushort y = (ushort)  loc.y;
			ushort hash = (ushort) loc.hash;

			outputFileFile.write(&x);
			outputFileFile.write(&y);
			outputFileFile.write(&hash);
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

	cout << "File read." << endl;
}

void FrankEncode::encode(){

	cout << "Encoding file." << endl;

	if(outputImage){
		writeImage();
	}
	else{
		writeFile();
	}
}

void FrankEncode::close(){
	image.close();
	plainFile.close();

	if(!outputImage){
		outputFileFile.close();
	}
}
