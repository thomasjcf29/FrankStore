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

#ifndef FRANKSTORE_FILETOOUTPUT_H
#define FRANKSTORE_FILETOOUTPUT_H

#include <string>
#include <fstream>

class FileToOutput{
private:
    std::string fileName;
    std::ofstream outputFile;
    bool valid = true;
public:
    FileToOutput();
    FileToOutput(std::string fileName);
    bool isValid();
    void write(unsigned short data);
    void writetest(const char* data);
    void close();
    void flush();
    int cal(char c);
};

#endif //FRANKSTORE_FILETOOUTPUT_H
