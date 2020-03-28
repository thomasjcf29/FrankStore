#include "CoverImage.h"
#include "CoverPixel.h"
#include "Converter.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>


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

struct compare
{
	string key;
	compare(string const &i): key(i) { }

	bool operator()(string const &i)
	{
		return (i == key);
	}
};

int main(int argc, char **argv) {

	if(!check(argc, argv)){
		cout << "Exiting Program" << endl;
		exit(2);
	}

    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Max Pixels: " << test.getMaxPixels() << endl;

    vector<string> pixelsUsed;
    CoverPixel pixels[1000];

    for(int i = 0; i < 1000; i++){
        bool invalid = true;
        do{
            int * loc = test.getRandomLocation();
            string x,y,key;
            x = to_string(loc[0]);
            y = to_string(loc[1]);
            key = x + "-" + y;

            if(any_of(pixelsUsed.begin(), pixelsUsed.end(), compare(key))){
                invalid = true;
                cout << "Element Found" << endl;
            }
            else{
                invalid = false;
                pixelsUsed.push_back(key);
                string colour = test.getHexColour(loc[0], loc[1]);
                pixels[i] = CoverPixel(loc[0], loc[1], colour);
            }

        } while(invalid);
    }

    test.close();
  	return 0;
}
