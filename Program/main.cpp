#include <iostream>
#include "CoverImage.h"

using namespace std;

int main() {
    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Pixel Colour: " << test.getHexColour(0, 0) << endl;
    cout << "Max Pixels: " << test.getMaxPixels() << endl;
    test.close();

    for(int x = 0; x < test.getWidth(); x++){
        for(int y = 0; y < test.getHeight(); y++){
            cout << "Pixel Colour, X: " << x << ", Y: " << y << ", Colour: " << test.getHexColour(x, y) << endl;
        }
    }

    return 0;
}
