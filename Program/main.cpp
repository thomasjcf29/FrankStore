#include <iostream>
#include <string.h>
#include "FrankEncode.h"
#include "FrankGenerator.h"

using namespace std;

/**
Function called to output to the screen the input options.
Should be called if there are wrong entries.
*/
void layout() {
  cout << "[ERROR]: Please provide arguments to this program." << endl;
  cout << "The correct arguments are:" << endl;
  cout << "To   encode: FrankStore encode <coverImage> <fileToEncode> <outputFile>" << endl;
  cout << "To   decode: FrankStore decode <coverImage> <fileToDecode> <outputFile>" << endl;
  cout << "To generate: FrankStore generate <width> <height> <outputFile>" << endl;
}

/**
Checks if the arguments provided are valid, displays to the user what they should be
if they are not.
@param int argc: The number of arguments provided.
@param char **argv: The arguments provided.
@return true if valid, false if not.
*/
bool check(int argc, char **argv) {
    if (argc == 5){
        if(!strcmp(argv[1], "encode") == 0 && !strcmp(argv[1], "decode") == 0 && !strcmp(argv[1], "generate") == 0){
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

    if(strcmp(argv[1], "encode") == 0){
        bool invalid = true;
        bool image = true;
        do{
            cout << "Would you like the output as an (i)mage or a (f)ile? Enter i or f:";
            string input;
            cin >> input;
            cout << endl;

            if(input == "f"){
                image = false;
                invalid = false;
            }
            else if(input == "i"){
                image = true;
                invalid = false;
            }
            else{
                cout << "Invalid input." << endl;
            }
        } while(invalid);

        FrankEncode encoder = FrankEncode(argv, image);
        encoder.encode();
        encoder.close();
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
