#ifndef FRANKSTORE_EXIFMANAGER_H
#define FRANKSTORE_EXIFMANAGER_H

#include <string.h>
#include <exiv2/exiv2.hpp>

class ExifManager{
private:
    bool valid = false;
    size_t length;
    Exiv2::Image* image;
public:
    ExifManager();
    ExifManager(char* imageLocation);
    ~ExifManager();
    std::string* getExifData();
    bool isValid();
    size_t getSize();
};

#endif //FRANKSTORE_EXIFMANAGER_H
