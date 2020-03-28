#include "CoverImage.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sha512.h"

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

	cout << "Argument Count: " << argc << endl;

    for(int i = 0; i < argc; i++){
      cout << "Argument (" << (i + 1) << "): " << argv[i] << endl;
    }

	if(!check(argc, argv))
		cout << "Exiting Program" << endl;
		exit(2);

    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Pixel Colour: " << test.getHexColour(0, 0) << endl;
    cout << "Max Pixels: " << test.getMaxPixels() << endl;
    test.close();

    for(int i = 0; i < 10; i++){
      int * hmm = test.getRandomLocation();
      cout << "X: " << hmm[0] << ", Y:" << hmm[1] << endl;
    }

    cout << "Argument Count: " << argc << endl;

    for(int i = 0; i < argc; i++){
      cout << "Argument (" << (i + 1) << "): " << argv[i] << endl;
    }

  	return 0;
}
