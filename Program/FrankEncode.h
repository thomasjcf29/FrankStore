#ifndef FRANKSTORE_FRANKENCODE_H
#define FRANKSTORE_FRANKENCODE_H

#include "CoverImage.h"
#include "CoverPixel.h"
#include <vector>
#include <string>

class FrankEncode{
private:
    CoverImage image;
    CoverPixel pixels[1000];
    std::vector<std::string> pixelsUsed;
    bool getPixels(int amount);
    bool replacePixel(int location);
    CoverPixel findPixel();
    bool checkCount(int pixel, std::string letter);
public:
    FrankEncode(char **argv);
};

#endif //FRANKSTORE_FRANKENCODE_H
