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
        int x, y, hash;
    };
#endif //FRANKSTORE_LOCATION

class CoverImage{
private:
    std::string location;
    Magick::Image img;
    bool valid = false;
    int maxPixels, availablePixels, curX, curY, incrementor, failedAttempts;
public:
    CoverImage(std::string file);
    CoverImage();
    std::string getHexColour(int x, int y);
    void close();
    int getWidth();
    int getHeight();
    int getMaxPixels();
    int* getNextLocation();
    bool isValid();
    void claimUsedPixel();
    void resetFailedAttempts();
    int getPixelsLeft();
    char* getHexCode(Location* hexCode, size_t bufferSize);
};

#endif //FRANKSTORE_COVERIMAGE_H
