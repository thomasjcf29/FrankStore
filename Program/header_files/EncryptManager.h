#ifndef FRANKSTORE_ENCRYPTMANAGER_H
#define FRANKSTORE_ENCRYPTMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>

class EncryptManager{
private:
    const signed char* tempSalt = "wD,Fvhjb2{y=5N^w";
    const unsigned char* salt = reinterpret_cast<unsigned char*>(tempSalt);
public:
    EncryptManager();
    static void PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes, char* hexResult, uint8_t* binResult);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
