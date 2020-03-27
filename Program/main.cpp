#include <iostream>
#include "FranksImage.h"

using namespace std;

int main() {
    FranksImage test = FranksImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Pixel Colour: " << test.getHexColour(0, 0) << endl;
    test.close();
    return 0;
}
