#ifndef FRANKSTORE_FRANKENCRYPT_H
#define FRANKSTORE_FRANKENCRYPT_H

#include "CoverImage.h"

enum RunTime {Nothing, Image, Password, ImageAndPassword};

class FrankEncrypt{
private:
    bool valid = false;
    CoverImage coverImage;
    RunTime action = Nothing;
    std::string encryptionKey;

    void getPassword();
public:
    FrankEncrypt(int argc, char **argv);
    bool isValid();
    void encrypt();
    void close();
};

#endif //FRANKSTORE_FRANKENCRYPT_H
