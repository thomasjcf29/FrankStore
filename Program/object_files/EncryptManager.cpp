/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

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

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
EncryptManager::EncryptManager(){
}

/**
Returns an object handling the encryption and decryption of files. Check is valid
before using it. (.isValid())
@param unsigned char* password: The password to use.
@param string inputFile: The file to read from.
@param string outputFIle: The file to write too.
*/
EncryptManager::EncryptManager(unsigned char* password, string inputFile, string outputFile){

    key = password;
    iv = new unsigned char[16];
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

/**
Returns the validity status of itself and the objects it has initalised.
@return validity status, if false do not use.
*/
bool EncryptManager::isValid(){
    return valid;
}

/**
Securely generates an IV of size 16 bytes. This will need to be done for encryption.
*/
void EncryptManager::generateIV(){
    cout << "[INFO]: Generating IV..." << endl;
    if(!RAND_bytes(iv, 16)){
        valid = false;
        cout << "[ERROR]: Generating IV, program cannot continue." << endl;
    }
    else{
        cout << "[INFO]: IV Generated." << endl;
    }
}

/**
Sets IV to the one specified, used during decryption.
@param unsigned char* readIV: The IV which has been read from file.
*/
void EncryptManager::setIV(unsigned char* readIV){
    iv = readIV;
}

/**
Loops through the input file and writes the encrypted bytes to output.
Depending on the file size this may take a while.
*/
void EncryptManager::encrypt(){
    cout << "[INFO]: Encrypting, this may take a while..." << endl;

    int len;
    int ciphertext_len;
    unsigned char ciphertext[32];

    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        cout << "[ERROR]: Cannot create cipher, exiting." << endl;
        exit(55);
    }

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)){
        cout << "[ERROR]: Cannot initialise encryption, exiting." << endl;
        exit(55);
    }

    //Write IV to File
    out.write(reinterpret_cast<char*>(iv), 16);

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
            ciphertext_len += len;
        }

        out.write(reinterpret_cast<char*>(ciphertext), ciphertext_len);

        delete [] data;
    }

    EVP_CIPHER_CTX_free(ctx);

    cout << "[INFO]: File encrypted." << endl;
}

/**
Loops through the input file and writes the plain text bytes to output.
Depending on the file size this may take a while.
*/
void EncryptManager::decrypt(){
    cout << "[INFO]: Decrypting, this may take a while..." << endl;

    int len;
    int plaintext_len;
    unsigned char ciphertext[16];
    unsigned char plaintext[32];
    bool finalLoop = false;

    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        cout << "[ERROR]: Cannot create cipher, exiting." << endl;
        exit(55);
    }

    //Get Total File Size
    in.seekg(0, ios::end);
    size_t fileSize = in.tellg();
    size_t leftToRead = fileSize;
    in.seekg(0, ios::beg);

    if(fileSize < 16 || (fileSize % 16) != 0){
        cout << "[ERROR]: Invalid input file!" << endl;
        exit(56);
    }

    cout << "[INFO]: Reading IV..." << endl;
    unsigned char* readIV = new unsigned char[16];
    in.read(reinterpret_cast<char*>(readIV), 16);
    leftToRead -= 16;
    setIV(readIV);
    cout << "[INFO]: IV Read." << endl;

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)){
        cout << "[ERROR]: Cannot initialise decryption, exiting." << endl;
        exit(56);
    }

    //Amount of cipher iterations, blocks must be 16 bytes
    size_t iterations = (size_t) ceil(leftToRead / 16.0);
    cout << "[INFO]: Total bytes: " << leftToRead << ", block iterations: " << iterations << endl;

    for(size_t i = 0; i < iterations; i++){
        //Last iteration
        if(i+1 == iterations){
            finalLoop = true;
        }

        in.read(reinterpret_cast<char*>(ciphertext), 16);

        if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, 16)){
            cout << "[ERROR]: Error decrypting info, exiting." << endl;
            exit(56);
        }

        plaintext_len = len;

        if(finalLoop){
            if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)){
                cout << "[ERROR]: Error decrypting final block, exiting." << endl;
                exit(55);
            }

            plaintext_len += len;
        }

        out.write(reinterpret_cast<char*>(plaintext), plaintext_len);
    }

    cout << "[INFO]: File decrypted." << endl;
}

/**
Deletes any raw pointers in memory to free up resources.
Should be called after you are finished with the object.
*/
void EncryptManager::close(){
    delete [] key;
    delete [] iv;
}

/**
Static method to take a variable sized input and return a fixed array of key bytes
to be used for encryption and decryption. The returned key size is 32 bytes.
@param const char* pass: The variable sized password.
@param int passlen: The length of the variable password.
@param int32_t: The amount of iterations to hash the password.
@return a fixed 32 byte sized key to be used for encryption.
*/
unsigned char* EncryptManager::PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations){
    const unsigned char* salt = (unsigned char*) tempSalt;
    unsigned char* digest = new unsigned char[32];
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, 16, iterations, EVP_sha256(), (uint32_t) 32, digest);
    return digest;
}
