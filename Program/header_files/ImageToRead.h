#ifndef FRANKSTORE_IMAGETOREAD_H
#define FRANKSTORE_IMAGETOREAD_H

#include <string>
#include <fstream>
#include <Magick++.h>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class ImageToRead{
private:
    std::string fileName;
    bool read;
    bool valid;
    size_t width = 0;
    size_t height = 0;
    size_t totalPixels = 0;
    size_t paddingPixel = 0;
    size_t readablePixels = 0;
    size_t pixelsReadSoFar = 0;
    size_t bufferSize = 0;
    size_t curX = 0;
    size_t curY = 0;
    Location buffer[1024];
    Magick::Image image;
public:
    ImageToRead();
    ImageToRead(std::string name);
    ~ImageToRead();
    bool isRead();
    bool isValid();
    size_t getBufferSize();
    int getWidth();
    int getHeight();
    Location* getNextPixels();
};

#endif //FRANKSTORE_IMAGETOREAD_H
