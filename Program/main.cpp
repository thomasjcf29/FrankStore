#include "CoverImage.h"
#include "CoverPixel.h"
#include "Converter.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

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

    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Max Pixels: " << test.getMaxPixels() << endl;

	int * loc = test.getRandomLocation();
	string colour = test.getHexColour(loc[0], loc[1]);

	CoverPixel f = CoverPixel(loc[0], loc[1], colour);
	cout << "X: " << loc[0] << ", Y: " << loc[1] << endl;
	cout << "Colour: " << colour << endl;
	cout << "Hash: " << f.getHash() << endl;

    test.close();
  	return 0;
}
