#include <iostream>
#include "CoverImage.h"

using namespace std;

int main() {
    CoverImage test = CoverImage("/home/thomasjcf21/download.jpg");
    cout << "Width: " << test.getWidth() << endl;
    cout << "Height: " << test.getHeight() << endl;
    cout << "Pixel Colour: " << test.getHexColour(0, 0) << endl;
    test.close();
    return 0;
}
