#ifndef FRANKSTORE_ENCRYPTMANAGER_H
#define FRANKSTORE_ENCRYPTMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>

class EncryptManager{
private:
    static constexpr const char* tempSalt = "wD,Fvhjb2{y=5N^w";
public:
    EncryptManager();
    static unsigned char* PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
