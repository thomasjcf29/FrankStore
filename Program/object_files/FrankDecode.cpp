#include "../header_files/FrankDecode.h"
#include "../header_files/CoverImage.h"
#include "../header_files/Converter.h"
#include "../header_files/CoverPixel.h"
#include "../header_files/ImageToOutput.h"
#include "../header_files/FileToOutput.h"
#include "../header_files/ImageToRead.h"
#include "../header_files/FileToDecode.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

/**
Creates a FrankDecode object, check the object is valid before proceeding. Mind you,
this object will just quit if the provided parameters are not valid!
@param char **argv: Array of user provided arguments from the command line.
@param bool isImage: Boolean for to test program if decode object is an image.
*/
FrankDecode::FrankDecode(char **argv, bool isImage){
    cout << "[INFO]: Opening cover image." << endl;
    image = CoverImage(argv[2]);

	isImageInput = isImage;

    if(isImageInput){
        cout << "[INFO]: Opening image to decode." << endl;
        inputImage = ImageToRead(argv[3]);

        if(!inputImage.isValid()){
            cout << "Could not read input image, exiting!" << endl;
            exit(3);
        }
    }
    else{
        cout << "[INFO]: Opening file to decode." << endl;
        inputFile = FileToDecode(argv[3]);

        if(!inputFile.isValid()){
            cout << "Could not read input file, exiting!" << endl;
            exit(3);
        }
    }

    if(!image.isValid()){
        cout << "Could not read cover file, closing program." << endl;
        exit(3);
    }

    cout << "[INFO]: Creating output file." << endl;
    outputFile = FileToOutput(argv[4]);

    if(!outputFile.isValid()){
        cout << "[ERROR]: Program exiting" << endl;
        exit(7);
    }

    valid = true;
}

/**
@return if the object is valid to be used: true if valid, false if not.
*/
bool FrankDecode::isValid(){
    return valid;
}

/**
Private method used to decode the image and write to a file.
*/
void FrankDecode::decodeImage(){
    bool moreToRead = true;
    do{
        Location* locations = inputImage.getNextPixels();
        const char* strHex = image.getHexCode(locations, inputImage.getBufferSize());
        outputFile.writetest(strHex);
        delete [] strHex;
        moreToRead = !inputImage.isRead();
    } while(moreToRead);

    outputFile.flush();
}

/**
Private method used to decode the file and write to a file.
*/
void FrankDecode::decodeFile(){
    bool moreToRead = true;
    do{
        Location* locations = inputFile.getNextPixels();
        const char* strHex = image.getHexCode(locations, inputFile.getBufferSize());
        outputFile.writetest(strHex);
        delete [] strHex;
        moreToRead = !inputFile.isRead();
    } while(moreToRead);

    outputFile.flush();
}

/**
Public method called to decode the file, will return once the file has been fully decoded.
This may take a while!
*/
void FrankDecode::decode(){
    cout << "[INFO]: Writing decode file." << endl;
    if(isImageInput){
        decodeImage();
    }
    else{
        decodeFile();
    }
    cout << "[INFO]: Decode file has been written." << endl;
}

/**
Public method used to close all resources this object has open.
*/
void FrankDecode::close(){
    if(!isImageInput){
        inputFile.close();
    }
    outputFile.close();
}
