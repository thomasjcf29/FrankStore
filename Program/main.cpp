#include "CoverImage.h"
#include <iostream>
#include <string.h>
#include "sha512.h"

using namespace std;

int main(int argc, char **argv) {
    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Pixel Colour: " << test.getHexColour(0, 0) << endl;
    cout << "Max Pixels: " << test.getMaxPixels() << endl;
    test.close();

    for(int x = 0; x < test.getWidth(); x++){
        for(int y = 0; y < test.getHeight(); y++){
            string colour = test.getHexColour(x, y);
            string sx,sy;
            sx = to_string(x);
            sy = to_string(y);
            string hash = sha512(sx + sy + colour);
            cout << "Pixel Colour, X: " << x << ", Y: " << y << ", Colour: "
                 << colour << ", Hash: " << hash << endl;
        }
    }

  	return 1;
}
