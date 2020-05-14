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

#ifndef FRANKSTORE_FRANKENCODE_H
#define FRANKSTORE_FRANKENCODE_H

#include "CoverImage.h"
#include "CoverPixel.h"
#include "FileToEncode.h"
#include "ImageToOutput.h"
#include "FileToOutput.h"
#include <vector>
#include <string>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class FrankEncode{
private:
    CoverImage image;
    FileToEncode plainFile;
    std::vector<CoverPixel> pixels;
    bool** pixelsUsed;
    ImageToOutput outputFileImage;
    FileToOutput outputFileFile;
    bool outputImage;
    int counter = 0;

    bool getPixels(int amount);
    void replacePixel(int location);
    CoverPixel findPixel();
    bool checkCount(int pixel, std::string letter);
    Location encodeLetter(std::string hashLetter);
    size_t calculateBestImageSize(size_t fileSize);
    void writeImage();
    void writeFile();
public:
    FrankEncode(char **argv, bool isImage);
    void encode();
    void close();
};

#endif //FRANKSTORE_FRANKENCODE_H
