#include "Converter.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

string Converter::rgb2hex(int r, int g, int b, bool with_head){
  stringstream ss;
  if (with_head)
    ss << "#";
  ss << std::hex << std::setw(6) << std::setfill('0') << (r << 16 | g << 8 | b );
  return ss.str();
}

int Converter::hex2int(string hex){
    return stoul(hex, nullptr, 16);
}
