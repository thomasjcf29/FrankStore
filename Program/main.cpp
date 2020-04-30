#include "header_files/FrankDecode.h"
#include "header_files/FrankEncode.h"
#include "header_files/FrankGenerator.h"
#include "header_files/FrankEncrypt.h"
#include <iostream>
#include <string.h>

using namespace std;

/**
Function called to output to the screen the input options.
Should be called if there are wrong entries.
*/
void layout() {
  cout << "[ERROR]: Please provide arguments to this program." << endl;
  cout << "The correct arguments are:" << endl;
  cout << "To   encode: FrankStore encode <coverImage> <fileToEncode> <outputFile> <image|file>" << endl;
  cout << "To   decode: FrankStore decode <coverImage> <fileToDecode> <outputFile> <image|file>" << endl;
  cout << "Image = output as an image, file = output as a file" << endl;
  cout << "To generate: FrankStore generate <width> <height> <outputFile>" << endl;
  cout << "There are multiple options for encryption, all result in the same output." << endl;
  cout << "If you are unsure refer to the documentation!" << endl;
  cout << "The following are valid options to encrypt/decrypt: " << endl;
  cout << "FrankStore <encrypt|decrypt> <fileToRead> <fileToOutput>" << endl;
  cout << "FrankStore <encrypt|decrypt> <fileToRead> <fileToOutput> password <password>" << endl;
  cout << "FrankStore <encrypt|decrypt> <fileToRead> <fileToOutput> image <imageFile>" << endl;
  cout << "FrankStore <encrypt|decrypt> <fileToRead> <fileToOutput> password image <imageFile>" << endl;
  cout << "FrankStore <encrypt|decrypt> <fileToRead> <fileToOutput> password <password> image <imageFile>" << endl;
}

/**
Checks if the arguments provided are valid, displays to the user what they should be
if they are not.
@param int argc: The number of arguments provided.
@param char **argv: The arguments provided.
@return true if valid, false if not.
*/
bool check(int argc, char **argv) {
    if (argc == 5 || argc == 6 || argc == 4 || argc == 7 || argc == 8){
        if(strcmp(argv[1], "encode") == 0 || strcmp(argv[1], "decode") == 0){
            if(argc != 6){
                layout();
                return false;
            }
            if(strcmp(argv[5], "file") != 0 && strcmp(argv[5], "image") != 0){
                cout << "[ERROR]: An invalid entry was submitted, file or image?" << endl;
                layout();
                return false;
            }
        }
        else if(strcmp(argv[1], "generate") == 0){
            if(argc != 5){
                layout();
                return false;
            }

            try{
                int i = stoi(argv[2]);
                int x = stoi(argv[3]);
            }
            catch(invalid_argument e){
                cout << "[ERROR]: An error was detected when trying to convert to an integer, please try again!" << endl;
                layout();
                return false;
            }
            catch(out_of_range e1){
                cout << "[ERROR]: Number is too large to be an integer!" << endl;
                layout();
                return false;
            }
        }
        else if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0){
            if(argc == 4){
                return true;
            }
            else if(argc == 6){
                if(strcmp(argv[4], "password") != 0 && strcmp(argv[4], "image") != 0){
                    cout << "[ERROR]: An invalid entry was submitted, password or image?" << endl;
                    layout();
                    return false;
                }
            }
            else if(argc == 7){
                if(strcmp(argv[4], "password") != 0 || strcmp(argv[5], "image") != 0){
                    cout << "[ERROR]: An invalid entry was submitted!" << endl;
                    layout();
                    return false;
                }
            }
            else if(argc == 8){
                if(strcmp(argv[4], "password") != 0 || strcmp(argv[6], "image") != 0){
                    cout << "[ERROR]: An invalid entry was submitted!" << endl;
                    layout();
                    return false;
                }
            }
            else{
                layout();
                return false;
            }
        }
        else{
            layout();
            return false;
        }
        return true;
    }
    else{
        layout();
        return false;
    }
}

/**
Main entry point to the program, handles all objects and offloads the
encoding / decoding / generating.
@param int argc: The number of user arguments provided.
@param char **argv: The arguments provided by the end user.
@return the error code of the application.
*/
int main(int argc, char **argv) {

	if(!check(argc, argv)){
		cout << "[ERROR]: Exiting Program" << endl;
		exit(2);
	}

    if(strcmp(argv[1], "encode") == 0 || strcmp(argv[1], "decode") == 0){
        bool image;

        if(strcmp(argv[5], "file") == 0){
            image = false;
        }
        else if(strcmp(argv[5], "image") == 0){
            image = true;
        }

        if(strcmp(argv[1], "encode") == 0){
            FrankEncode encoder = FrankEncode(argv, image);
            encoder.encode();
            encoder.close();
        }
        else{
            FrankDecode decoder = FrankDecode(argv, image);
            decoder.decode();
            decoder.close();
        }
    }

    else if(strcmp(argv[1], "generate") == 0){
        FrankGenerator generator = FrankGenerator(argv);
        if(!generator.isValid()){
            cout << "[ERROR]: Exiting program";
            exit(10);
        }
        generator.generateData();
    }

    else if(strcmp(argv[1], "encrypt") == 0 || strcmp(argv[1], "decrypt") == 0){
        FrankEncrypt encryptor = FrankEncrypt(argc, argv);

        if(!encryptor.isValid()){
            cout << "[ERROR]: Exiting program";
            exit(10);
        }

        if(strcmp(argv[1], "encrypt") == 0){
            encryptor.encrypt();
        }
        else{
            encryptor.decrypt();
        }
        encryptor.close();
    }

  	return 0;
}
