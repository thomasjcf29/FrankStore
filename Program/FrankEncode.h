#ifndef FRANKSTORE_FRANKENCODE_H
#define FRANKSTORE_FRANKENCODE_H

#include "CoverImage.h"
#include "CoverPixel.h"
#include "FileToEncode.h"
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
    
    bool getPixels(int amount);
    void replacePixel(int location);
    CoverPixel findPixel();
    bool checkCount(int pixel, std::string letter);
    Location encodeLetter(std::string hashLetter);
public:
    FrankEncode(char **argv);
    void close();
};

#endif //FRANKSTORE_FRANKENCODE_H
