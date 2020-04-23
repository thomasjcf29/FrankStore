#include "../header_files/CoverImage.h"
#include "../header_files/CoverPixel.h"
#include "../header_files/Converter.h"
#include <iostream>
#include <Magick++.h>
#include <sodium.h>
#include <sstream>
#include <cstring>

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

    int halfWidth = (int) ceil(getWidth() / 2);
	bool invalid = true;
	do{
		incrementor = randombytes_uniform((uint32_t) getWidth());
		if(incrementor >= halfWidth){
			invalid = false;
		}

	} while(invalid);

    curX = 0;
    curY = 0;
    failedAttempts = 0;
};

/**
@return The width of the image.
*/
size_t CoverImage::getWidth(){
    if(!valid)
        return -1;
    return img.columns();
}

/**
@return The height of the image.
*/
size_t CoverImage::getHeight(){
    if(!valid)
        return -1;
    return img.rows();
}

/**
@return The total amount of pixels in the image (width * height).
*/
size_t CoverImage::getMaxPixels(){
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
    int red = (int) (px.red() * 255);
    int green = (int) (px.green() * 255);
    int blue = (int) (px.blue() * 255);

    return Converter::rgb2hex(red, green, blue, true);
}

/**
The next pixel in the image according to the incrementor.
@return Memory location to the int array of the coords (x,y).
If called 10 times without resetting failed attempts the incrementor will change.
The pixel returned is not unique, you must make sure you are doing it yourself.
Make sure to delete the array returned or else they'll be memory issues.
*/
size_t * CoverImage::getNextLocation(){
    size_t * location = new size_t[2];

    if(curX >= getWidth()){
        size_t leftOver = curX - getWidth();
        curX = leftOver;
        curY++;
    }

    if(curY >= getHeight()){
        curY = 0;
        failedAttempts++;
    }

    if(failedAttempts >= 10){
        int halfWidth = (int) ceil(incrementor / 2);
        int minHalfWidth = (int) ceil(incrementor / 4);
    	bool invalid = true;
    	do{
    		incrementor = (randombytes_uniform(halfWidth) + 1);
    		if(incrementor >= minHalfWidth){
    			invalid = false;
    		}

    	} while(invalid);

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
Returns the overall hex code for the specified array of locations.
@param Location* locations: The array of locations you would like decoded.
@param size_t bufferSize: The size of the array.
@return the char array of the hex code.
WARNING: You need to delete [] the array provided back to prevent a memeory leak!
*/
char* CoverImage::getHexCode(Location* locations, size_t bufferSize){
    stringstream ss;

    for(size_t i = 0; i < bufferSize; i++){
        string pxHex = getHexColour(locations[i].x, locations[i].y);
        CoverPixel px = CoverPixel(locations[i].x, locations[i].y, pxHex);
        ss << px.getLetterAt(locations[i].hash);
    }

    string str = ss.str();
    char *c = new char [str.size() + 1];
    strcpy(c, str.c_str());
    return c;
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
size_t CoverImage::getPixelsLeft(){
    return availablePixels;
}

/**
To be called as class is closed, it gets rid of memeory objects.
*/
void CoverImage::close(){
    location.clear();
}
