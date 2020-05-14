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

#ifndef FRANKSTORE_FILETOENCODE_H
#define FRANKSTORE_FILETOENCODE_H

#include <fstream>
#include <string>

class FileToEncode{
private:
    std::ifstream inputFile;
    bool isRead = false;
    bool valid = true;
    char buffer[1024];
    size_t totalLength;
    size_t bufferSize;
    size_t readSoFar = 0;
public:
    FileToEncode();
    FileToEncode(std::string file);
    char* getNextBytes();
    bool isFileRead();
    bool isValid();
    size_t getBufferSize();
    size_t getTotalSize();
    size_t getReadSoFar();
    void close();
};

#endif //FRANKSTORE_FILETOENCODE_H
