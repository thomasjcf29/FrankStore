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

#ifndef FRANKSTORE_IMAGETOOUTPUT_H
#define FRANKSTORE_IMAGETOOUTPUT_H

#include <Magick++.h>
#include <string>

class ImageToOutput{
private:
    Magick::Image file;
    size_t setX, setY, curX, curY;
    std::string fileName;
    bool valid = true;
    bool written = false;
public:
    ImageToOutput();
    ImageToOutput(std::string fileToWrite, size_t imageX, size_t imageY);
    bool isValid();
    bool isWritten();
    void write();
    void updatePixel(Magick::Color color);
};

#endif //FRANKSTORE_IMAGETOOUTPUT_H
