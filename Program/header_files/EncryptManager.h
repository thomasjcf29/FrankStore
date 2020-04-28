#ifndef FRANKSTORE_ENCRYPTMANAGER_H
#define FRANKSTORE_ENCRYPTMANAGER_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>

class EncryptManager{
private:
    static constexpr const char* tempSalt = "wD,Fvhjb2{y=5N^w";
    unsigned char* key;
    ifstream in;
    ofstream out;
    bool valid;
public:
    EncryptManager();
    EncryptManager(unsigned char* password, std::string inputFile, std::string outputFile);
    bool isValid();
    static unsigned char* PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
