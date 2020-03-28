#include "FrankEncode.h"
#include "CoverImage.h"
#include "CoverPixel.h"
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

struct compare{
	string key;
	compare(string const &i): key(i) { }

	bool operator()(string const &i)
	{
		return (i == key);
	}
};

FrankEncode::FrankEncode(char **argv){
    image = CoverImage(argv[2]);

    if(!image.isValid()){
        cout << "Could not read cover file, closing program." << endl;
        exit(3);
    }

    if(image.getMaxPixels() < 1000){
        cout << "[ERROR] The image you have chosen is too small." << endl;
        cout << "You need at least 1000 pixels in an image." << endl;
        exit(4);
    }

    getPixels(1000);

    image.close();
}

CoverPixel FrankEncode::findPixel(){
    bool invalid = true;
    do{
        int * loc = image.getRandomLocation();
        string x,y,key;
        x = to_string(loc[0]);
        y = to_string(loc[1]);
        key = x + "-" + y;

        if(any_of(pixelsUsed.begin(), pixelsUsed.end(), compare(key))){
            invalid = true;
        }
        else{
            invalid = false;
            pixelsUsed.push_back(key);
            string colour = image.getHexColour(loc[0], loc[1]);
            image.claimUsedPixel();
            return CoverPixel(loc[0], loc[1], colour);
        }

    } while(invalid);
}

bool FrankEncode::getPixels(int pixelAmount){
    if(image.getPixelsLeft() < pixelAmount)
        return false;

    for(int i = 0; i < pixelAmount; i++){
        pixels[i] = findPixel();
    }

    return true;
}

bool FrankEncode::replacePixel(int location){
    if(image.getPixelsLeft() < 1)
        return false;
    pixels[location] = findPixel();
    return true;
}
