#include "../header_files/FrankEncrypt.h"
#include "../header_files/ConsoleController.h"
#include <iostream>

using namespace std;

FrankEncrypt::FrankEncrypt(int argc, char **argv){
    cout << "[INFO]: Setting up encryptor." << endl;

    //Password - Not Specified
    if(argc == 4){
        action = Password;
        ConsoleController::SetStdinEcho(false);

        string password;
        cin >> password;

        ConsoleController::SetStdinEcho(true);

        cout << "Password: " << password << endl;
    }
    //Image or Password Specified
    else if(argc == 6){
        if(strcmp(argv[4], "password") != 0){
            action = Password;
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
    }
    //Image and Password (Password Specified)
    else if(argc == 8){
        action = ImageAndPassword;
        coverImage = CoverImage(argv[7]);
        valid = coverImage.isValid();
    }
}

bool FrankEncrypt::isValid(){
    return valid;
}
