#ifndef FRANKSTORE_EXIFMANAGER_H
#define FRANKSTORE_EXIFMANAGER_H

#include <string.h>
#include <exiv2/exiv2.hpp>

class ExifManager{
private:
    bool valid;
    size_t length;
public:
    ExifManager();
    ExifManager(std::string imageLocation);
    std::string* getExifData();
    bool isValid();
    size_t getSize();
    void close();
};

#endif //FRANKSTORE_EXIFMANAGER_H
