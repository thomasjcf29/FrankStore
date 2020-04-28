#include "../header_files/EncryptManager.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

using namespace std;

EncryptManager::EncryptManager(){

}

EncryptManager::EncryptManager(unsigned char* password, string inputFile, string outputFile){

    int size = sizeof(key) / sizeof(key[0]);

    cout << "Size: " << size << endl;

    if(size != 32){
        cout << "[WARNING]: The input key size is not 32, is there a problem?" << endl;
    }

    key = password;
    in = ifstream(inputFile);
    out = ofstream(outputFile);

    if(!in){
        cout << "[ERROR]: Unable to open file to read, do you have permission?" << endl;
        valid = false;
        return;
    }

    if(!out){
        cout << "[ERROR]: Unable to create file to write, do you have permission?" << endl;
        valid = false;
        return;
    }

    valid = true;
}

bool EncryptManager::isValid(){
    return valid;
}

void EncryptManager::generateIV(){

}

void EncryptManager::setIV(){

}

unsigned char* EncryptManager::PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes)
{
    const unsigned char* salt = (unsigned char*) tempSalt;
    unsigned char* digest = new unsigned char[outputBytes];
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, sizeof(salt), iterations, EVP_sha256(), outputBytes, digest);
    return digest;
}
