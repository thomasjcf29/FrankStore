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

#ifndef FRANKSTORE_FRANKGENERATOR_H
#define FRANKSTORE_FRANKGENERATOR_H

#include "ImageToOutput.h"
#include <string>
#include <iostream>

class FrankGenerator{
private:
    ImageToOutput image;
    size_t width, height;
    bool valid;
public:
    FrankGenerator(char **argv);
    void generateData();
    bool isValid();
};

#endif //FRANKSTORE_FRANKGENERATOR_H
