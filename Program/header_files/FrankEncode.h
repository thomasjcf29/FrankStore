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
