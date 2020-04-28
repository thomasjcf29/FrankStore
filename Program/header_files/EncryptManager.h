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
    std::ifstream in;
    std::ofstream out;
    bool valid = false;
public:
    EncryptManager();
    EncryptManager(unsigned char* password, std::string inputFile, std::string outputFile);
    bool isValid();
    void generateIV();
    void setIV();
    static unsigned char* PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes);
};

#endif //FRANKSTORE_ENCRYPTMANAGER_H
