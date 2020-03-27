//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_FRANKSIMAGE_H
#define FRANKSTORE_FRANKSIMAGE_H

#include <iostream>
#include <Magick++.h>
#include "Converter.h"

using namespace std;
using namespace Magick;

class FranksImage{
private:
    string location;
    Magick::Image img;
    bool valid = false;
public:
    FranksImage(string file);
    string getHexColour(int x, int y);
    void close();
    int getWidth();
    int getHeight();
};

FranksImage::FranksImage(string file){
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

int FranksImage::getWidth(){
    if(!valid)
        return -1;
    return img.columns();
}

int FranksImage::getHeight(){
    if(!valid)
        return -1;
    return img.rows();
}

string FranksImage::getHexColour(int x, int y){
    if(!valid)
        return "-1";
    ColorRGB px = img.pixelColor(x, y);
    int red = px.red() * 255;
    int green = px.green() * 255;
    int blue = px.blue() * 255;

    return Converter::rgb2hex(red, green, blue, true);
}

#endif //FRANKSTORE_FRANKSIMAGE_H