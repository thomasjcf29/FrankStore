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

#ifndef FRANKSTORE_FRANKDECODE_H
#define FRANKSTORE_FRANKDECODE_H

#include "CoverImage.h"
#include "CoverPixel.h"
#include "ImageToOutput.h"
#include "FileToOutput.h"
#include "FileToDecode.h"
#include "ImageToRead.h"
#include <string>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class FrankDecode{
private:
    CoverImage image;
    bool isImageInput;
    bool valid;
    int counter = 0;
    FileToOutput outputFile;
    ImageToRead inputImage;
    FileToDecode inputFile;

    void decodeImage();
    void decodeFile();
public:
    FrankDecode(char **argv, bool isImage);
    void decode();
    void close();
    bool isValid();
};

#endif //FRANKSTORE_FRANKDECODE_H
