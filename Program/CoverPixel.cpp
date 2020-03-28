#include "CoverPixel.h"
#include "Converter.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sha512.h"
#include <sodium.h>
#include "vector"

using namespace std;

CoverPixel::CoverPixel(){

}

CoverPixel::CoverPixel(int lx, int ly, string lcolour){
    intX = lx;
    intY = ly;

    string sx = to_string(lx);
    string sy = to_string(ly);
    x = sx;
    y = sy;
    colour = lcolour;
    hash = sha512(x + y + colour);

    char myString[32];
    randombytes_buf(myString, 32);

    for(int i = 0; i < hash.length(); i++){
        string letter = hash.substr(i, 1);
        int number = Converter::hex2int(letter);
        letters[number].push_back(i);
    }
}

int CoverPixel::getLetterCount(string letter){
    int arrayPos = Converter::hex2int(letter);
    return letters[arrayPos].size();
}

int CoverPixel::getLetter(string letter){
    int arrayPos = Converter::hex2int(letter);
    int location = randombytes_uniform(letters[arrayPos].size());
    int hashLocation = letters[arrayPos].at(location);

    letters[arrayPos].erase(letters[arrayPos].begin() + location);

    return hashLocation;
}

string CoverPixel::getHash(){
    return hash;
}

int CoverPixel::getX(){
    return intX;
}

int CoverPixel::getY(){
    return intY;
}
