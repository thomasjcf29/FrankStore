#ifndef FRANKSTORE_FRANKGENERATOR_H
#define FRANKSTORE_FRANKGENERATOR_H

#include "ImageToOutput.h"
#include <string>
#include <iostream>

class FrankGenerator{
private:
    ImageToOutput image;
    size_t width, height;
    bool valid;
public:
    FrankGenerator(char **argv);
    void generateData();
    bool isValid();
};

#endif //FRANKSTORE_FRANKGENERATOR_H
