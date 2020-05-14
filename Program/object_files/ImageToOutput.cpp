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

#include "../header_files/ImageToOutput.h"
#include <iostream>

using namespace std;
using namespace Magick;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
ImageToOutput::ImageToOutput(){

}

/**
Creates the image to output and sets up the relevant internal variables as required.
@param string fileToWrite: The name of the file to write.
@param size_t imageX: The width of the output image.
@param size_t imageY: The height of the output image.
*/
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
        valid = false;
        return;
    }

    cout << "[INFO]: Created image." << endl;
}

/**
@return if the object is valid, true if it is, false if it is not.
*/
bool ImageToOutput::isValid(){
    return valid;
}

/**
@return if the image has been completely written (all pixels have been changed).
*/
bool ImageToOutput::isWritten(){
    return written;
}

/**
Writes the image buffer to the file, useful for managing memory during large images.
*/
void ImageToOutput::write(){
    file.write(fileName);
}

/**
Updates the next pixel in the image with the colour requested.
@param Magick::Color color: The color you would like of the pixel.
*/
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
