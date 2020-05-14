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
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_CONVERTER_H
#define FRANKSTORE_CONVERTER_H

#include <string>

class Converter{
public:
    static std::string rgb2hex(int r, int g, int b, bool with_head);
    static double* hex2rgb(std::string hex);
    static int hex2int(std::string hex);
    static std::string int2hex(int);
    static std::string char2hex(char* bytes, size_t size);
    static int char2int(char c);
private:
    Converter() {}
};

#endif //FRANKSTORE_CONVERTER_H
