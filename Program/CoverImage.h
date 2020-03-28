//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_COVERIMAGE_H
#define FRANKSTORE_COVERIMAGE_H

#include <Magick++.h>
#include <sodium.h>
#include <string>

class CoverImage{
private:
    std::string location;
    Magick::Image img;
    bool valid = false;
    int maxPixels;
public:
    CoverImage(std::string file);
    CoverImage();
    std::string getHexColour(int x, int y);
    void close();
    int getWidth();
    int getHeight();
    int getMaxPixels();
    int* getRandomLocation();
};

#endif //FRANKSTORE_COVERIMAGE_H
