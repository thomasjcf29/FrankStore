#ifndef FRANKSTORE_FRANKENCODE_H
#define FRANKSTORE_FRANKENCODE_H

#include "CoverImage.h"
#include "CoverPixel.h"
#include "FileToEncode.h"
#include "ImageToOutput.h"
#include <vector>
#include <set>
#include <string>

struct Location{
    int x, y, hash;
};

class FrankEncode{
private:
    CoverImage image;
    FileToEncode plainFile;
    std::vector<CoverPixel> pixels;
    std::set<std::string> pixelsUsed;
    ImageToOutput outputFile;
    int counter = 0;

    bool getPixels(int amount);
    void replacePixel(int location);
    CoverPixel findPixel();
    bool checkCount(int pixel, std::string letter);
    Location encodeLetter(std::string hashLetter);
    size_t calculateBestImageSize(size_t fileSize);
public:
    FrankEncode(char **argv);
    void encode();
    void close();
};

#endif //FRANKSTORE_FRANKENCODE_H
