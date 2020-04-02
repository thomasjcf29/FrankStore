#include "FrankGenerator.h"
#include "ImageToOutput.h"
#include "Converter.h"
#include "Magick++.h"
#include <sodium.h>

using namespace std;

/**
Constructor which creates the output image, and initalises class variables.
@param char **arv: The input parameters from the console.
*/
FrankGenerator::FrankGenerator(char **argv){
    width = stoi(argv[2]);
    height = stoi(argv[3]);
    string fileName = argv[4];

    char myString[32];
    randombytes_buf(myString, 32);

    image = ImageToOutput(fileName, width, height);
    valid = image.isValid();
}

/**
Generates all the pixels in the image size that the end user requested, you should
check the validity of the object before calling this method.
This method will return once the image is generated, this could take a while!
*/
void FrankGenerator::generateData(){
    while(!image.isWritten()){
		int random = randombytes_uniform(16777216);
		double* rgb = Converter::hex2rgb(Converter::int2hex(random));

		Magick::Color color = Magick::ColorRGB(rgb[0], rgb[1], rgb[2]);

		delete [] rgb;
		image.updatePixel(color);
	}

	image.write();
}

/**
@return if the object is valid, true if the output image could be created or false if it could not.
*/
bool FrankGenerator::isValid(){
    return valid;
}
