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

int * CoverImage::getRandomLocation(){
    int * location = new int[2];
    char myString[32];

    randombytes_buf(myString, 32);

    location[0] = randombytes_uniform(getWidth());
    location[1] = randombytes_uniform(getHeight());

    return location;
}

bool CoverImage::isValid(){
    return valid;
}

void CoverImage::claimUsedPixel(){
    availablePixels--;
}

int CoverImage::getPixelsLeft(){
    return availablePixels;
}

void CoverImage::close(){
    location.clear();
}
