#include "ImageToOutput.h"
#include <iostream>

using namespace std;
using namespace Magick;

ImageToOutput::ImageToOutput(){

}

ImageToOutput::ImageToOutput(string fileToWrite, size_t imageX, size_t imageY){
    cout << "[INFO]: Setting image parameters." << endl;
    setX = imageX;
    setY = imageY;
    curX = 0;
    curY = 0;

    fileName = fileToWrite;

    cout << "[INFO]: Creating image map." << endl;
    Color white = ColorRGB(255,255,255);
    Geometry geo = Geometry(imageX, imageY);

    cout << "[INFO]: Trying to create image." << endl;
    try{
        file = Image(geo, white);
        file.write(fileName);
    }
    catch(exception &error){
        cout << "[ERROR]: Can't write file, do you have permission?" << endl;
        cout << "[ERROR]: " << error.what() << endl;
        valid = false;
        return;
    }

    cout << "[INFO]: Created image." << endl;
}

bool ImageToOutput::isValid(){
    return valid;
}

bool ImageToOutput::isWritten(){
    return written;
}

void ImageToOutput::write(){
    file.write(fileName);
}

void ImageToOutput::updatePixel(Color color){

    if(curX >= setX){
        curX = 0;
        curY++;
    }

    if(curY > setY){
        cout << "[ERROR]: This error shouldn't be possible?" << endl;
        exit(911);
    }

    file.pixelColor(curX, curY, color);
    curX++;

    if(curX >= setX && (curY + 1) >= setY){
        written = true;
        return;
    }
}
