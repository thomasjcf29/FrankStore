#include "header_files/FrankDecode.h"
#include "header_files/FrankEncode.h"
#include "header_files/FrankGenerator.h"
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
  cout << "To generate: FrankStore generate <width> <height> <outputFile>" << endl;
  cout << "Image = output as an image, file = output as a file" << endl;
}

/**
Checks if the arguments provided are valid, displays to the user what they should be
if they are not.
@param int argc: The number of arguments provided.
@param char **argv: The arguments provided.
@return true if valid, false if not.
*/
bool check(int argc, char **argv) {
    if (argc == 5 || argc == 6){
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
		cout << "Exiting Program" << endl;
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

  	return 0;
}
