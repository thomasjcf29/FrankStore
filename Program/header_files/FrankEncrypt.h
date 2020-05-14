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

#ifndef FRANKSTORE_FRANKENCRYPT_H
#define FRANKSTORE_FRANKENCRYPT_H

#include "CoverImage.h"
#include "ExifManager.h"
#include "EncryptManager.h"

enum RunTime {Nothing, Image, Password, ImageAndPassword};

class FrankEncrypt{
private:
    bool valid = false;
    ExifManager exifManager;
    EncryptManager encryptManager;
    RunTime action = Nothing;
    std::string encryptionKey;
    std::string imageKey;
    std::string overallKey;
    void getPassword();
    void getImageInfo();
public:
    FrankEncrypt(int argc, char **argv);
    bool isValid();
    void encrypt();
    void decrypt();
    void close();
};

#endif //FRANKSTORE_FRANKENCRYPT_H
