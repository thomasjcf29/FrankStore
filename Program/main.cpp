#include <iostream>
#include <string.h>
#include "FrankEncode.h"

using namespace std;

void layout() {
  cout << "[ERROR]: Please provide arguments to this program." << endl;
  cout << "The correct arguments are:" << endl;
  cout << "To encode: FrankStore encode <coverImage> <fileToEncode> <outputFile>" << endl;
  cout << "To decode: FrankStore decode <coverImage> <fileToDecode> <outputFile>" << endl;
}

bool check(int argc, char **argv) {
    if (argc == 5){
        if(!strcmp(argv[1], "encode") == 0 && !strcmp(argv[1], "decode") == 0){
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

  	return 0;
}
