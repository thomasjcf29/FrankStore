//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_COVERIMAGE_H
#define FRANKSTORE_COVERIMAGE_H

#include <iostream>
#include <Magick++.h>
#include "Converter.h"

using namespace std;
using namespace Magick;

class CoverImage{
private:
    string location;
    Magick::Image img;
    bool valid = false;
public:
    CoverImage(string file);
    string getHexColour(int x, int y);
    void close();
    int getWidth();
    int getHeight();
};

CoverImage::CoverImage(string file){
    location = file;
    try{
        img.read(file);
        valid = true;
    }
    catch( exception &error_ )
    {
        cout << "Problem opening file: " << error_.what() << endl;
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

void CoverImage::close(){
    location.clear();
}

#endif //FRANKSTORE_COVERIMAGE_H
