//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_COVERIMAGE_H
#define FRANKSTORE_COVERIMAGE_H

#include <Magick++.h>
#include <sodium.h>
#include <string>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class CoverImage{
private:
    std::string location;
    Magick::Image img;
    bool valid = false;
    int incrementor, failedAttempts;
    size_t maxPixels, availablePixels, curX, curY;
public:
    CoverImage(std::string file);
    CoverImage();
    std::string getHexColour(size_t x, size_t y);
    void close();
    size_t getWidth();
    size_t getHeight();
    size_t getMaxPixels();
    size_t* getNextLocation();
    bool isValid();
    void claimUsedPixel();
    void resetFailedAttempts();
    size_t getPixelsLeft();
    char* getHexCode(Location* hexCode, size_t bufferSize);
};

#endif //FRANKSTORE_COVERIMAGE_H
