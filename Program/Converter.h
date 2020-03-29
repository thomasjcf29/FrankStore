//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_CONVERTER_H
#define FRANKSTORE_CONVERTER_H

#include <string>

class Converter{
public:
    static std::string rgb2hex(int r, int g, int b, bool with_head);
    static int* hex2rgb(std::string hex);
    static int hex2int(std::string hex);
    static std::string int2hex(int);
    static std::string char2hex(char* bytes, size_t size);
private:
    Converter() {}
};

#endif //FRANKSTORE_CONVERTER_H
