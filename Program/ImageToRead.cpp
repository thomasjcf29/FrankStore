#include "ImageToRead.h"
#include "Converter.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace Magick;

ImageToRead::ImageToRead(){

}

ImageToRead::ImageToRead(string name){
    fileName = name;
    try{
        image.read(fileName);
        cout << "File read." << endl;
        width = image.columns();
        height = image.rows();
        totalPixels = image.columns() * image.rows();
        valid = true;
    }
    catch( exception &error_ )
    {
        cout << "Problem opening file: " << "Does the file exist?" << endl;
        valid = false;
        return;
    }

    cout << "Removing padding.....";

    for(size_t i = 0; i < width; i++){
        for(size_t y = 0; y < height; y++){
            ColorRGB col = image.pixelColor(i, y);
            int r = col.red() * 255;
            int g = col.green() * 255;
            int b = col.blue() * 255;

            if(r == 255 && g == 255 && b == 255){
                paddingPixel = (y * width) + i;
                goto stop;
            }
        }
    }

    stop:
    cout << "done." << endl;
    readablePixels = paddingPixel;

    if(readablePixels % 3 != 0){
        cout << "Total Pixels: " << totalPixels << endl;
        cout << "Padding Pixels: " << paddingPixel << endl;
        cout << "Readable Pixels: " << readablePixels << endl;
        cout << "[ERROR] Problem with decode image, invalid sizes!" << endl;
        valid = false;
    }
}

ImageToRead::~ImageToRead(){
}

size_t ImageToRead::getBufferSize(){
    return bufferSize;
}

bool ImageToRead::isRead(){
    return read;
}

bool ImageToRead::isValid(){
    return valid;
}

int ImageToRead::getWidth(){
    return width;
}

int ImageToRead::getHeight(){
    return height;
}

Location* ImageToRead::getNextPixels(){
    if(((readablePixels / 3) - (pixelsReadSoFar / 3)) > 1024){
        bufferSize = 1024;
    }
    else{
        bufferSize = (readablePixels / 3) - (pixelsReadSoFar / 3);
    }

    pixelsReadSoFar += bufferSize * 3;

    for(int i = 0; i < bufferSize; i++){
        Location loc;

        int details[3];

        for(int ii = 0; ii < 3; ii++){
            if(curX >= width){
                curX = 0;
                curY++;
            }

            ColorRGB color = image.pixelColor(curX, curY);
            int r = color.red() * 255;
            int g = color.green() * 255;
            int b = color.blue() * 255;

            string hex = Converter::rgb2hex(r, g, b, false);
            int number = Converter::hex2int(hex);

            details[ii] = number;

            curX++;
        }

        loc = Location{details[0], details[1], details[2]};

        buffer[i] = loc;
    }

    if(pixelsReadSoFar >= readablePixels){
        read = true;
    }

    return buffer;
}
