#include "FrankEncode.h"
#include "CoverImage.h"
#include "CoverPixel.h"
#include <iostream>
#include <string.h>
#include "sha512.h"
#include <vector>
#include <sodium.h>

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

    char myString[32];
    randombytes_buf(myString, 32);

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

    for(int i = 0; i < 100000; i++){
        //cout << endl;
        Location test = encodeLetter("a");
    }

    image.close();
}

CoverPixel FrankEncode::findPixel(){
    bool invalid = true;
    do{
        int * loc = image.getRandomLocation();
        string x,y,key;
        x = to_string(loc[0]);
        y = to_string(loc[1]);
        key = sha512(x + "-" + y);

        pair<std::set<string>::iterator,bool> result = pixelsUsed.insert(key);

        if(result.second){
            invalid = false;
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
        pixels.push_back(findPixel());
    }

    return true;
}

void FrankEncode::replacePixel(int location){
    if(pixels.size() <= 1){
        cout << "[ERROR]: Ran out of pixels, please try with a bigger image!" << endl;
        exit(5);
    }

    if(image.getPixelsLeft() < 1){
        cout << "[INFO]: You are running low on pixels!" << endl;
        pixels.erase(pixels.begin() + location);
        pixels.shrink_to_fit();
        return;
    }

    pixels[location] = findPixel();
}

bool FrankEncode::checkCount(int pixelLoc, string letter){
    int amount = pixels[pixelLoc].getLetterCount(letter);
    if(amount < 1){
        replacePixel(pixelLoc);
        return false;
    }
    return true;
}

Location FrankEncode::encodeLetter(string hashLetter){

    bool valid;
    int pixelToUse;
    CoverPixel pixel;

    do{
        pixelToUse = randombytes_uniform(pixels.size());
        valid = checkCount(pixelToUse, hashLetter);
    } while(!valid);

    pixel = pixels[pixelToUse];

    int x = pixel.getX();
    int y = pixel.getY();
    int hashLocation = pixel.getLetter(hashLetter);

    pixels[pixelToUse] = pixel;

    return Location{x, y, hashLocation};
}
