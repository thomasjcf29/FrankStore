#include "../header_files/EncryptManager.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <ctype.h>
#include <openssl/crypto.h>

void EncryptManager::PBKDF2_HMAC_SHA_256(const char* pass, int passlen, const unsigned char* salt, int saltlen, int32_t iterations, uint32_t outputBytes, char* hexResult, uint8_t* binResult)
{
    unsigned int i;
    unsigned char* digest = new unsigned char[outputBytes];
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, saltlen, iterations, EVP_sha256(), outputBytes, digest);
    for (i = 0; i < sizeof(digest); i++)
    {
        sprintf(hexResult + (i * 2), "%02x", 255 & digest[i]);
        binResult[i] = digest[i];
    };
    delete [] digest;
}
