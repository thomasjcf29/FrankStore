#include "Converter.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

char const hexHolder[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};

string Converter::rgb2hex(int r, int g, int b, bool with_head){
  stringstream ss;
  if (with_head)
    ss << "#";
  ss << std::hex << std::setw(6) << std::setfill('0') << (r << 16 | g << 8 | b );
  return ss.str();
}

int* Converter::hex2rgb(string hex){
    int* rgb = new int[3];
    stringstream ss;
    string str;

    // Drop a hash if the value has one
    if (hex[0] == '#') {
    	hex.erase(0,1);
    }

    int size = hex.size();

    for (int i = 0; i < 3; i++) {
    	// Determine 3 or 6 character format.
    	if (size == 3) { str = string(2, hex[i]); }
    	else if (size == 6) { str = hex.substr(i * 2, 2); }
    	else { break; }

    	ss << std::hex << str;
    	ss >> rgb[i];
    	ss.clear();
    }

    return rgb;
}

int Converter::hex2int(string hex){
    return stoul(hex, nullptr, 16);
}

string Converter::int2hex(int number){
    stringstream stream;
    stream << std::hex << number;
    return stream.str();
}

string Converter::char2hex(char* bytes, size_t size){
    string result;
    for(int i = 0; i < size; i++){
        const char ch = bytes[i];
        result.append(&hexHolder[(ch  & 0xF0) >> 4], 1);
        result.append(&hexHolder[ch & 0xF], 1);
    }
    return result;
}
