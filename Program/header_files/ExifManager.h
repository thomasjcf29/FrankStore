/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

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
