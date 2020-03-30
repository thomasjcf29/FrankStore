#include "FrankGenerator.h"
#include "ImageToOutput.h"
#include "Converter.h"
#include "Magick++.h"
#include <sodium.h>

using namespace std;

FrankGenerator::FrankGenerator(char **argv){
    width = stoi(argv[2]);
    height = stoi(argv[3]);
    string fileName = argv[4];

    char myString[32];
    randombytes_buf(myString, 32);

    image = ImageToOutput(fileName, width, height);
    valid = image.isValid();
}

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

bool FrankGenerator::isValid(){
    return valid;
}
