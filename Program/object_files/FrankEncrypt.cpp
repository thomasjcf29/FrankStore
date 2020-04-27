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

        valid = true;
    }
    //Image or Password Specified
    else if(argc == 6){
        if(strcmp(argv[4], "password") == 0){
            action = Password;
            encryptionKey = string(argv[5]);

            cout << "Password: " << encryptionKey << endl;

            valid = true;
        }
        else if(strcmp(argv[4], "image") == 0){
            action = Image;
            (&exifManager)->~ExifManager();
            new (&exifManager) ExifManager(argv[5]);
            valid = exifManager.isValid();
        }
    }
    //Image and Password (Password Not Specified)
    else if (argc == 7){
        action = ImageAndPassword;
        (&exifManager)->~ExifManager();
        new (&exifManager) ExifManager(argv[6]);
        valid = exifManager.isValid();

        getPassword();

        cout << "Password: " << encryptionKey << endl;
    }
    //Image and Password (Password Specified)
    else if(argc == 8){
        action = ImageAndPassword;
        (&exifManager)->~ExifManager();
        new (&exifManager) ExifManager(argv[7]);
        valid = exifManager.isValid();

        encryptionKey = string(argv[5]);
        cout << "Password: " << encryptionKey << endl;
    }

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
