#include "../header_files/EncryptManager.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

unsigned char* EncryptManager::PBKDF2_HMAC_SHA_256(const char* pass, int passlen, int32_t iterations, uint32_t outputBytes)
{
    const unsigned char* salt = (unsigned char*) tempSalt;
    unsigned char* digest = new unsigned char[outputBytes];
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, sizeof(salt), iterations, EVP_sha256(), outputBytes, digest);
    return digest;
}
