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

#ifndef FRANKSTORE_ENCRYPTMANAGER_H
#define FRANKSTORE_ENCRYPTMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>

class EncryptManager{
private:
    static constexpr const char* tempSalt = "wD,Fvhjb2{y=5N^w";
    unsigned char* key = NULL;
    unsigned char* iv = NULL;
    std::ifstream in;
    std::ofstream out;
    bool valid = false;

    void setIV(unsigned char* readIV);
public:
    EncryptManager();
    EncryptManager(unsigned char* password, std::string inputFile, std::string outputFile);
    bool isValid();
    void generateIV();
    void encrypt();
    void decrypt();
    void close();
    static unsigned char* PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
