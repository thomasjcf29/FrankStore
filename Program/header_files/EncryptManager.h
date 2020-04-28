#ifndef FRANKSTORE_ENCRYPTMANAGER_H
#define FRANKSTORE_ENCRYPTMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>

class EncryptManager{
private:
public:
    EncryptManager();
    static void void PBKDF2_HMAC_SHA_256(const char* pass, int passlen, const unsigned char* salt, int saltlen, int32_t iterations, uint32_t outputBytes, char* hexResult, uint8_t* binResult);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
