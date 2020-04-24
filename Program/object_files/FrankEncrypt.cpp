#include "../header_files/FrankEncrypt.h"

FrankEncrypt::FrankEncrypt(int argc, char **argv){
    cout << "[INFO]: Setting up encryptor." << endl;

    //Password - Not Specified
    if(argc == 4){
        action = Password;
    }
    //Image or Password Specified
    else if(argc == 6){
        action = Image;
        coverImage = CoverImage(argv[5]);
        valid = coverImage.isValid();
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
