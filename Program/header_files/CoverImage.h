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
