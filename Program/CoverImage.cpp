#include "CoverImage.h"
#include <iostream>
#include <Magick++.h>
#include "Converter.h"
#include <sodium.h>

using namespace std;
using namespace Magick;

/**
A blank constructor for the object, this is only to be used to allow it to be
referenced in a header file. Do not use this object based on this.
*/
CoverImage::CoverImage(){

}

/**
Constructor for the object, it tries to open the file specified and will set
the validity status of the object. It initalises class variables to be used.
@param string file: The image file you would like to open.
*/
CoverImage::CoverImage(string file){
    location = file;
    try{
        img.read(file);
        maxPixels = img.columns() * img.rows();
        availablePixels = maxPixels;
        valid = true;
    }
    catch( exception &error_ )
    {
        cout << "Problem opening file: " << "Does the file exist?" << endl;
        valid = false;
    }

    char myString[32];
    randombytes_buf(myString, 32);

    int halfWidth = ceil(getWidth() / 2);
	bool invalid = true;
	do{
		incrementor = randombytes_uniform(getWidth());
		if(incrementor >= halfWidth){
			invalid = false;
            cout << "[INFO]: Initial Incrementor: " << incrementor << endl;
		}

	} while(invalid);

    curX = 0;
    curY = 0;
    failedAttempts = 0;
};

/**
@return The width of the image.
*/
int CoverImage::getWidth(){
    if(!valid)
        return -1;
    return img.columns();
}

/**
@return The height of the image.
*/
int CoverImage::getHeight(){
    if(!valid)
        return -1;
    return img.rows();
}

/**
@return The total amount of pixels in the image (width * height).
*/
int CoverImage::getMaxPixels(){
    return maxPixels;
}

/**
The hex colour of the pixel at the given coordinates.
@param int x: The x coordinate.
@param int y: The y coordinate.
@return The hex colour of the pixel.
*/
string CoverImage::getHexColour(int x, int y){
    if(!valid)
        return "-1";
    if(x > (getWidth() - 1))
        return "ERROR: The requested width is too large.";
    if(y > (getHeight() - 1))
        return "ERROR: The requested height is too large.";

    ColorRGB px = img.pixelColor(x, y);
    int red = px.red() * 255;
    int green = px.green() * 255;
    int blue = px.blue() * 255;

    return Converter::rgb2hex(red, green, blue, true);
}

/**
The next pixel in the image according to the incrementor.
@return Memory location to the int array of the coords (x,y).
If called 10 times without resetting failed attempts the incrementor will change.
The pixel returned is not unique, you must make sure you are doing it yourself.
Make sure to delete the array returned or else they'll be memory issues.
*/
int * CoverImage::getNextLocation(){
    int * location = new int[2];

    if(curX >= getWidth()){
        int leftOver = curX - getWidth();
        curX = leftOver;
        curY++;
    }

    if((curY + 1) >= getHeight()){
        curY = 0;
        failedAttempts++;
    }

    if(failedAttempts >= 10){
        cout << "New Incrementor Requested: ";
        int halfWidth = ceil(incrementor / 2);
        int minHalfWidth = ceil(incrementor / 4);
    	bool invalid = true;
    	do{
    		incrementor = (randombytes_uniform(halfWidth) + 1);
    		if(incrementor >= minHalfWidth){
    			invalid = false;
    		}

    	} while(invalid);

        cout << incrementor << endl;

        resetFailedAttempts();
    }

    location[0] = curX;
    location[1] = curY;

    curX = curX + incrementor;

    return location;
}

/**
@return if the object is valid, could the image be opened?
*/
bool CoverImage::isValid(){
    return valid;
}

/**
Claim a used pixel, the number of available pixels decreases.
*/
void CoverImage::claimUsedPixel(){
    availablePixels--;
}

/**
Reset the amount of failed attempts to 0, to be used when a pixel returned is unique.
*/
void CoverImage::resetFailedAttempts(){
    failedAttempts = 0;
}

/**
@return the amount of pixels left that are unique.
*/
int CoverImage::getPixelsLeft(){
    return availablePixels;
}

/**
To be called as class is closed, it gets rid of memeory objects.
*/
void CoverImage::close(){
    location.clear();
}
