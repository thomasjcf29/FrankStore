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
