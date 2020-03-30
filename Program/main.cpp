#include <iostream>
#include <string.h>
#include "FrankEncode.h"
#include "FrankGenerator.h"

using namespace std;

void layout() {
  cout << "[ERROR]: Please provide arguments to this program." << endl;
  cout << "The correct arguments are:" << endl;
  cout << "To   encode: FrankStore encode <coverImage> <fileToEncode> <outputFile>" << endl;
  cout << "To   decode: FrankStore decode <coverImage> <fileToDecode> <outputFile>" << endl;
  cout << "To generate: FrankStore generate <width> <height> <outputFile>" << endl;
}

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

int main(int argc, char **argv) {

	if(!check(argc, argv)){
		cout << "Exiting Program" << endl;
		exit(2);
	}

    if(strcmp(argv[1], "encode") == 0){
        FrankEncode encoder = FrankEncode(argv);
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
