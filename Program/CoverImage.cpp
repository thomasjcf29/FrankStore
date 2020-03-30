#include "CoverImage.h"
#include <iostream>
#include <Magick++.h>
#include "Converter.h"
#include <sodium.h>

using namespace std;
using namespace Magick;

CoverImage::CoverImage(){

}

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
            cout << "Initial Incrementor: " << incrementor << endl;
		}

	} while(invalid);

    int curX = 0;
    int curY = 0;
    int failedAttempts = 0;
};

int CoverImage::getWidth(){
    if(!valid)
        return -1;
    return img.columns();
}

int CoverImage::getHeight(){
    if(!valid)
        return -1;
    return img.rows();
}

int CoverImage::getMaxPixels(){
    return maxPixels;
}

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

int * CoverImage::getNextLocation(){
    int * location = new int[2];

    if(curX  >= getWidth()){
        // cout << "Out Of Bounds: " << curX << endl;
        // cout << "Width: " << getWidth() << endl;
        int leftOver = curX - getWidth();
        cout << "Left Over: " << leftOver << endl;
        curX = leftOver;
        // cout << "X: " << curX << endl;
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

    // cout << "X: " << curX << endl;
    // cout << "Y: " << curY << endl;

    curX = curX + incrementor;

    return location;
}

bool CoverImage::isValid(){
    return valid;
}

void CoverImage::claimUsedPixel(){
    availablePixels--;
}

void CoverImage::resetFailedAttempts(){
    failedAttempts = 0;
}

int CoverImage::getPixelsLeft(){
    return availablePixels;
}

void CoverImage::close(){
    location.clear();
}
