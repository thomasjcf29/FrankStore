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

#ifndef FRANKSTORE_IMAGETOREAD_H
#define FRANKSTORE_IMAGETOREAD_H

#include <string>
#include <fstream>
#include <Magick++.h>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class ImageToRead{
private:
    std::string fileName;
    bool read;
    bool valid;
    size_t width = 0;
    size_t height = 0;
    size_t totalPixels = 0;
    size_t paddingPixel = 0;
    size_t readablePixels = 0;
    size_t pixelsReadSoFar = 0;
    size_t bufferSize = 0;
    size_t curX = 0;
    size_t curY = 0;
    Location buffer[1024];
    Magick::Image image;
public:
    ImageToRead();
    ImageToRead(std::string name);
    ~ImageToRead();
    bool isRead();
    bool isValid();
    size_t getBufferSize();
    size_t getWidth();
    size_t getHeight();
    Location* getNextPixels();
};

#endif //FRANKSTORE_IMAGETOREAD_H
