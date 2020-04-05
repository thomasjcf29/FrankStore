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
        int x, y, hash;
    };
#endif //FRANKSTORE_LOCATION

class FrankDecode{
private:
    CoverImage image;
    bool isImageInput;
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
};

#endif //FRANKSTORE_FRANKDECODE_H
