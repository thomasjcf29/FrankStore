/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../header_files/FrankGenerator.h"
#include "../header_files/ImageToOutput.h"
#include "../header_files/Converter.h"
#include <Magick++.h>
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
