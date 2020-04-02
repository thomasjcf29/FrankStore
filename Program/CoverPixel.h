//
// Created by Thomas on 28/03/2020.
//

#ifndef FRANKSTORE_COVERPIXEL_H
#define FRANKSTORE_COVERPIXEL_H

#include <string>
#include <vector>

class CoverPixel{
private:
    std::string x, y, hash, colour;
    int intX, intY;
    std::vector<int> letters[16];
public:
    ~CoverPixel();
    CoverPixel();
    CoverPixel(int x, int y, std::string colour);
    int getLetter(std::string letter);
    std::string getHash();
    int getLetterCount(std::string letter);
    int getX();
    int getY();
};

#endif //FRANKSTORE_COVERPIXEL_H
