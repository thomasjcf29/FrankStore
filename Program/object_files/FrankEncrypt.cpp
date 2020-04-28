#include "../header_files/FrankEncrypt.h"
#include "../header_files/ConsoleController.h"
#include "../header_files/EncryptManager.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace std;

FrankEncrypt::FrankEncrypt(int argc, char **argv){
    cout << "[INFO]: Setting up encryptor." << endl;

    //Get rid of current ExifManager - new one will be made if required.
    (&exifManager)->~ExifManager();

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

    //char* pass = new char [overallKey.length()+1];
    //unsigned char* password = reinterpret_cast<unsigned char*>(pass);
    //strcpy(pass, overallKey.c_str());

    //cout << pass << endl;
    //delete [] pass;

    cout << "[INFO]: Encryption Manager setup up." << endl;
}

bool FrankEncrypt::isValid(){
    return valid;
}

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

    delete exifData;

    imageKey = tempData.str();
}
