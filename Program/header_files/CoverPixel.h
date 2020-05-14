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

//
// Created by Thomas on 28/03/2020.
//

#ifndef FRANKSTORE_COVERPIXEL_H
#define FRANKSTORE_COVERPIXEL_H

#include <string>
#include <vector>

class CoverPixel{
private:
    std::string x, y, hash, colour;
    size_t sizeX, sizeY;
    std::vector<int> letters[16];
public:
    ~CoverPixel();
    CoverPixel();
    CoverPixel(size_t x, size_t y, std::string colour);
    int getLetter(std::string letter);
    std::string getHash();
    size_t getLetterCount(std::string letter);
    std::string getLetterAt(int location);
    size_t getX();
    size_t getY();
};

#endif //FRANKSTORE_COVERPIXEL_H
