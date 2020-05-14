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

#ifndef FRANKSTORE_FILETODECODE_H
#define FRANKSTORE_FILETODECODE_H

#include <fstream>
#include <string>

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        size_t x, y;
        int hash;
    };
#endif //FRANKSTORE_LOCATION

class FileToDecode{
private:
    std::ifstream inputFile;
    std::string fileName;
    bool read;
    bool valid;
    size_t totalBytes = 0;
    size_t totalActualBytes = 0;
    size_t bytesReadSoFar = 0;
    size_t bufferSize = 0;
    Location buffer[1024];
public:
    FileToDecode();
    FileToDecode(std::string name);
    bool isValid();
    bool isRead();
    size_t getBufferSize();
    size_t getTotalBytes();
    size_t getBytesRead();
    Location* getNextPixels();
    void close();
};

#endif //FRANKSTORE_FILETODECODE_H
