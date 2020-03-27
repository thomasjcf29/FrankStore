//
// Created by Thomas on 26/03/2020.
//

#ifndef FRANKSTORE_CONVERTER_H
#define FRANKSTORE_CONVERTER_H

#include <sstream> 
#include <iomanip>

using namespace std;

class Converter{
public:
    static string rgb2hex(int r, int g, int b, bool with_head);
private:
    Converter() {}
};

string Converter::rgb2hex(int r, int g, int b, bool with_head){
  stringstream ss;
  if (with_head)
    ss << "#";
  ss << std::hex << std::setw(6) << std::setfill('0') << (r << 16 | g << 8 | b );
  return ss.str();
}

#endif //FRANKSTORE_CONVERTER_H