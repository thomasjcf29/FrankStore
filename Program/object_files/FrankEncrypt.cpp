#include "../header_files/FrankEncrypt.h"
#include "../header_files/ConsoleController.h"
#include "../header_files/EncryptManager.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

/**
Returns an object ready to encrypt an decrypt data. Any password or images are
automatically gotten from the user if required. Therefore this constructor can cause
an unlimited wait time until the user enters different information. Before using this
object, check it is valid using the .isValid() method. If it returns false, do not use it.
@param int argc: The arg count, passed from main.
@param char **argv: The arguments, passed from main.
*/
FrankEncrypt::FrankEncrypt(int argc, char **argv){
    cout << "[INFO]: Setting up encryptor." << endl;

    //Get rid of current ExifManager - new one will be made if required.
    (&exifManager)->~ExifManager();
    (&encryptManager)->~EncryptManager();

    //Password - Not Specified
    if(argc == 4){
        action = Password;
        getPassword();
        overallKey = encryptionKey;
        valid = true;
    }
    //Image or Password Specified
    else if(argc == 6){
        if(strcmp(argv[4], "password") == 0){
            action = Password;
            encryptionKey = string(argv[5]);
            overallKey = encryptionKey;
            valid = true;
        }
        else if(strcmp(argv[4], "image") == 0){
            action = Image;
            new (&exifManager) ExifManager(argv[5]);
        }
    }
    //Image and Password (Password Not Specified)
    else if (argc == 7){
        action = ImageAndPassword;
        new (&exifManager) ExifManager(argv[6]);
        getPassword();
    }
    //Image and Password (Password Specified)
    else if(argc == 8){
        action = ImageAndPassword;
        new (&exifManager) ExifManager(argv[7]);
        encryptionKey = string(argv[5]);
    }

    if(action == ImageAndPassword || action == Image){
        valid = exifManager.isValid();
        if(valid){
            getImageInfo();

            if(action == ImageAndPassword){
                overallKey = encryptionKey + ":" + imageKey;
            }
            else{
                overallKey = imageKey;
            }
        }
    }

    char* pass = new char [overallKey.length()+1];
    strcpy(pass, overallKey.c_str());

    unsigned char* key = EncryptManager::PBKDF2_HMAC_SHA_256(pass, ((int) overallKey.length())+1, 300000);

    delete [] pass;

    new (&encryptManager) EncryptManager(key, argv[2], argv[3]);

    //Do not want to change to true if already false.
    if(valid){
        valid = encryptManager.isValid();
    }

    cout << "[INFO]: Encryption Manager setup up." << endl;
}

/**
@return If the object is valid, true if valid, false if not.
*/
bool FrankEncrypt::isValid(){
    return valid;
}

/**
Gets password from user (if not specified), uses a none echo return prompt to hide
the password during typing.
*/
void FrankEncrypt::getPassword(){
    string tempPassword;
    string checkPassword;
    bool invalid = true;

    do{
        cout << "Please enter an encryption key: ";
        ConsoleController::SetStdinEcho(false);

        cin >> tempPassword;

        ConsoleController::SetStdinEcho(true);

        cout << endl << "Please verify password: ";
        ConsoleController::SetStdinEcho(false);

        cin >> checkPassword;

        ConsoleController::SetStdinEcho(true);

        cout << endl << endl;

        if(tempPassword.compare(checkPassword) == 0){
            cout << "[INFO]: Password Accepted." << endl << endl;
            encryptionKey = tempPassword;
            tempPassword.erase();
            checkPassword.erase();
            invalid = false;
        }
        else{
            cout << "[ERROR]: Passwords do not match, please reenter." << endl << endl;
            tempPassword.erase();
            checkPassword.erase();
        }

    } while(invalid);
}

/**
Get the EXIF Data from the image and place it into a string containing the image key.
*/
void FrankEncrypt::getImageInfo(){
    string* exifData = exifManager.getExifData();

    if(exifData == NULL){
        valid = false;
        return;
    }

    size_t size = exifManager.getSize();
    stringstream tempData;

    for(size_t i = 0; i < size; i++){
        tempData << exifData[i];
    }

    delete [] exifData;

    imageKey = tempData.str();
}

/**
Begin encryption of the file, will not return until the file is encrypted.
The program will exit if there is a problem during encryption.
*/
void FrankEncrypt::encrypt(){
    encryptManager.generateIV();

    if(!encryptManager.isValid()){
        cout << "[ERROR]: Cannot encrypt... exiting." << endl;
        exit(55);
    }

    encryptManager.encrypt();
}

/**
Begin decryption of the file, will not return until the file is decrypted.
The program will exit if there is a problem during decryption.
*/
void FrankEncrypt::decrypt(){
    //IV will be handled when decrypting the file.
    encryptManager.decrypt();
}

/**
Tidy up resources of any raw pointers that may exist.
*/
void FrankEncrypt::close(){
    encryptManager.close();
}
