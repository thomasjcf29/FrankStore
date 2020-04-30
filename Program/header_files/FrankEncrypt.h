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
    ~FrankEncrypt();
    bool isValid();
    void encrypt();
    void close();
};

#endif //FRANKSTORE_FRANKENCRYPT_H
