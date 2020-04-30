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
