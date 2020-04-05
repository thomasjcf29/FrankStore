#include "FrankDecode.h"
#include "CoverImage.h"
#include "Converter.h"
#include "CoverPixel.h"
#include "ImageToOutput.h"
#include "FileToOutput.h"
#include "ImageToRead.h"
#include "FileToDecode.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

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
}

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

void FrankDecode::decodeFile(){
    bool moreToRead = true;
    do{
        Location* locations = inputFile.getNextPixels();
        moreToRead = !inputFile.isRead();
    } while(moreToRead);
}

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
