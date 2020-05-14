/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../header_files/ImageToRead.h"
#include "../header_files/Converter.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace Magick;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
ImageToRead::ImageToRead(){

}

/**
Constructor used to create an ImageToRead object, it will open the image based
on the file name passed in. It will calculate the pixel to stop at, in both cases
it may take a while to complete and will not return until done so.
@param string name: The file location of the image you would like to open.
Do not use this object until checking it is valid, use isValid().
*/
ImageToRead::ImageToRead(string name){
    fileName = name;
    try{
        image.read(fileName);
        cout << "[INFO]: File read." << endl;
        width = image.columns();
        height = image.rows();
        totalPixels = image.columns() * image.rows();
        valid = true;
    }
    catch( exception &error_ )
    {
        cout << "[ERROR]: Problem opening file: " << "Does the file exist?" << endl;
        valid = false;
        return;
    }

    cout << "[INFO]: Removing padding.....";

    for(size_t i = 0; i < width; i++){
        for(size_t y = 0; y < height; y++){
            ColorRGB col = image.pixelColor(i, y);
            int r = (int) (col.red() * 255);
            int g = (int) (col.green() * 255);
            int b = (int) (col.blue() * 255);

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
        cout << "[INFO]: Total Pixels: " << totalPixels << endl;
        cout << "[INFO]: Padding Pixels: " << paddingPixel << endl;
        cout << "[INFO]: Readable Pixels: " << readablePixels << endl;
        cout << "[ERROR] Problem with decode image, invalid sizes!" << endl;
        valid = false;
    }
}

/**
Destructor for the object, currently there are no objects that can be closed.
*/
ImageToRead::~ImageToRead(){
}

/**
@return the current buffer size, the amount of locations read in at once. This
can be anywhere between 0 and 1024. Depending on how many pixels were read.
*/
size_t ImageToRead::getBufferSize(){
    return bufferSize;
}

/**
@return if the current object has been completly read: true if so, false if not.
*/
bool ImageToRead::isRead(){
    return read;
}

/**
@return if the current object is valid: true if so, false if not.
*/
bool ImageToRead::isValid(){
    return valid;
}

/**
@return the width of the decode image.
*/
size_t ImageToRead::getWidth(){
    return width;
}

/**
@return the height of the decode image.
*/
size_t ImageToRead::getHeight(){
    return height;
}

/**
@return an array of locations to decode, read straight from the file.
Use getBufferSize() to determine how big that array is.
*/
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

        size_t details[3];

        for(int ii = 0; ii < 3; ii++){
            if(curX >= width){
                curX = 0;
                curY++;
            }

            ColorRGB color = image.pixelColor(curX, curY);
            int r = (int) (color.red() * 255);
            int g = (int) (color.green() * 255);
            int b = (int) (color.blue() * 255);

            string hex = Converter::rgb2hex(r, g, b, false);
            size_t number = Converter::hex2int(hex);

            details[ii] = number;

            curX++;
        }

        loc = Location{details[0], details[1], (int) details[2]};

        buffer[i] = loc;
    }

    if(pixelsReadSoFar >= readablePixels){
        read = true;
    }

    return buffer;
}
