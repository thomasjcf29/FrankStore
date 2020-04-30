#include "../header_files/EncryptManager.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>

using namespace std;

EncryptManager::EncryptManager(){

}

EncryptManager::EncryptManager(unsigned char* password, string inputFile, string outputFile){

    key = password;
    in = ifstream(inputFile, ifstream::binary);
    out = ofstream(outputFile, ofstream::binary);

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
    cout << "[INFO]: Generating IV..." << endl;
    if(!RAND_bytes(iv, 16)){
        valid = false;
        cout << "[EROR]: Generating IV, program cannot continue." << endl;
    }
    else{
        cout << "[INFO]: IV Generated." << endl;
    }
}

void EncryptManager::setIV(){

}

void EncryptManager::encrypt(){
    cout << "[INFO]: Encrypting, this may take a while..." << endl;

    int len;
    int ciphertext_len;
    unsigned char ciphertext[16];

    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        cout << "[ERROR]: Cannot create cipher, exiting." << endl;
        exit(55);
    }

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)){
        cout << "[ERROR]: Cannot initialise encryption, exiting." << endl;
        exit(55);
    }

    //Get Total File Size
    in.seekg(0, ios::end);
    size_t fileSize = in.tellg();
    in.seekg(0, ios::beg);

    size_t leftToRead = fileSize;
    size_t readThisTime = 0;
    bool finalLoop = false;

    //Amount of cipher iterations, blocks must be 16 bytes
    size_t iterations = (size_t) ceil(fileSize / 16.0);

    cout << "[INFO]: Total bytes: " << fileSize << ", block iterations: " << iterations << endl;

    for(size_t i = 0; i < iterations; i++){
        //Last iteration
        if(i+1 == iterations){
            readThisTime = leftToRead;
            leftToRead -= leftToRead;
            finalLoop = true;
        }
        //There will always be 16 bytes if not last iteration.
        else{
            leftToRead -= 16;
            readThisTime = 16;
        }

        unsigned char* data = new unsigned char[readThisTime];

        in.read(reinterpret_cast<char*>(data), readThisTime);

        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, data, (int) readThisTime)){
            cout << "[ERROR]: Error encrypting info, exiting." << endl;
            exit(55);
        }

        ciphertext_len = len;

        if(finalLoop){
            if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)){
                cout << "[ERROR]: Error encrypting final block, exiting." << endl;
                exit(55);
            }
        }
        
        ciphertext_len = len;

        //out.write(reinterpret_cast<char*>(ciphertext), 16);

        cout << "Data: ";
        for(int y = 0; y < readThisTime; y++){
            cout << data[y];
        }
        cout << endl;

        cout << "Encrypted: ";
        for(int y = 0; y < ciphertext_len; y++){
            cout << ciphertext[y];
        }
        cout << endl;
        cout << "Left To Read: " << leftToRead << endl;

        delete [] data;
    }

    EVP_CIPHER_CTX_free(ctx);

    cout << "[INFO]: File encrypted." << endl;
}

unsigned char* EncryptManager::PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations){
    const unsigned char* salt = (unsigned char*) tempSalt;
    unsigned char* digest = new unsigned char[32];
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, 16, iterations, EVP_sha256(), (uint32_t) 32, digest);
    return digest;
}
