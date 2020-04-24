#include "../header_files/FrankEncrypt.h"
#include "../header_files/ConsoleController.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

FrankEncrypt::FrankEncrypt(int argc, char **argv){
    cout << "[INFO]: Setting up encryptor." << endl;

    //Password - Not Specified
    if(argc == 4){
        action = Password;

        getPassword();

        cout << "Password: " << encryptionKey << endl;
    }
    //Image or Password Specified
    else if(argc == 6){
        if(strcmp(argv[4], "password") != 0){
            action = Password;
            encryptionKey = string(argv[5]);

            cout << "Password: " << encryptionKey << endl;
        }
        else if(strcmp(argv[4], "image") != 0){
            action = Image;
            coverImage = CoverImage(argv[5]);
            valid = coverImage.isValid();
        }
    }
    //Image and Password (Password Not Specified)
    else if (argc == 7){
        action = ImageAndPassword;
        coverImage = CoverImage(argv[6]);
        valid = coverImage.isValid();

        getPassword();

        cout << "Password: " << encryptionKey << endl;
    }
    //Image and Password (Password Specified)
    else if(argc == 8){
        action = ImageAndPassword;
        coverImage = CoverImage(argv[7]);
        valid = coverImage.isValid();

        encryptionKey = string(argv[5]);

        cout << "Password: " << encryptionKey << endl;
    }
}

bool FrankEncrypt::isValid(){
    return valid;
}

void FrankEncrypt::getPassword(){
    cout << "Please enter an encryption key: ";

    ConsoleController::SetStdinEcho(false);

    cin >> encryptionKey;

    ConsoleController::SetStdinEcho(true);

    cout << endl << endl;
    cout << "Password Accepted." << endl << endl;
}
