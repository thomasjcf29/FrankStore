#include "CoverPixel.h"
#include "Converter.h"

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "sha512.h"
#include <sodium.h>
#include "vector"

using namespace std;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
CoverPixel::CoverPixel(){

}

/**
Destructor for this class, when the object gets deleted it clears memory objects
and closes resources.
*/
CoverPixel::~CoverPixel(){

    for(int i = 0; i < 16; i++){
        vector<int>().swap(letters[i]);
    }
}

/**
Constructor for the CoverPixel class, returning back a valid object.
@param int lx: The x location of the pixel.
@param int ly: The y location of the pixel.
@param string lcolour: The colour of the pixel.
*/
CoverPixel::CoverPixel(int lx, int ly, string lcolour){
    intX = lx;
    intY = ly;

    string sx = to_string(lx);
    string sy = to_string(ly);
    x = sx;
    y = sy;
    colour = lcolour;
    hash = sha512(x + "-" + y + "-" + colour);

    char myString[32];
    randombytes_buf(myString, 32);

    for(int i = 0; i < hash.length(); i++){
        string letter = hash.substr(i, 1);
        int number = Converter::hex2int(letter);
        letters[number].push_back(i);
    }
}

/**
Returns the letter at the specified hash location.
@param int location: The location of the hash letter you would like.
@return The string of the hash letter.
*/
string CoverPixel::getLetterAt(int location){
    return hash.substr(location, 1);
}

/**
@param The string hex character to be checked.
@return the count left of the hex character provided.
*/
int CoverPixel::getLetterCount(string letter){
    int arrayPos = Converter::hex2int(letter);
    return letters[arrayPos].size();
}

/**
Returns the position of the hex character you want to encode in the hash.
@param string letter: The hex character you want to encode.
@return int location of the hex character.
*/
int CoverPixel::getLetter(string letter){
    int arrayPos = Converter::hex2int(letter);
    vector<int> locations = letters[arrayPos];
    int location = randombytes_uniform(locations.size());
    int hashLocation = locations[location];

    locations.erase(locations.begin() + location);

    letters[arrayPos] = locations;

    return hashLocation;
}

/**
@return the overall hash of the object.
*/
string CoverPixel::getHash(){
    return hash;
}

/**
@return the X coordinates of the object.
*/
int CoverPixel::getX(){
    return intX;
}

/**
@return the Y coordinates of the object.
*/
int CoverPixel::getY(){
    return intY;
}
