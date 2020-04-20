#include "../header_files/Converter.h"
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

char const hexHolder[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};

/**
Converts RGB to a hex colour representation.
@param int r: The int colour in red.
@param int g: The int colour in green.
@param int b: The int colour in blue.
@param bool with_head: Do you want the output with a # at the start?
@return An hex code in a string object.
*/
string Converter::rgb2hex(int r, int g, int b, bool with_head){
  stringstream ss;
  if (with_head)
    ss << "#";
  ss << std::hex << std::uppercase << std::setw(6) << std::setfill('0') << (r << 16 | g << 8 | b );
  return ss.str();
}

/**
Converts a hex colour code back to the rgb colours.
@param string hex: The hex colour to convert.
@return A pointer to a double array, holding 3 doubles (R,G,B).
Make sure to delete the pointer returned after use.
*/
double* Converter::hex2rgb(string hex){
    int* rgb = new int[3];
    double* dblRGB = new double[3];
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
        dblRGB[i] = rgb[i] / 255.0;
    	ss.clear();
    }

    delete [] rgb;

    return dblRGB;
}

/**
Converts a hex code to its integer representation.
@param stirng hex: The hex to convert to an integer.
@return An integer representation of the hex.
*/
int Converter::hex2int(string hex){
    return stoul(hex, nullptr, 16);
}

/**
Converts an integer to its hex representation.
@param int number: The number to convert to hex.
@return An hex representation of the integer.
*/
string Converter::int2hex(int number){
    stringstream stream;
    stream << "#";
    stream << std::hex << std::setw(6) << std::setfill('0') << number;
    return stream.str();
}

/**
Converts an character array to hex.
@param char* bytes: The char pointer to the array of bytes.
@param size_t size: The size of the array.
@return string: The hex code representation of the characters.
*/
string Converter::char2hex(char* bytes, size_t size){
    string result;
    for(int i = 0; i < size; i++){
        const char ch = bytes[i];
        result.append(&hexHolder[(ch  & 0xF0) >> 4], 1);
        result.append(&hexHolder[ch & 0xF], 1);
    }
    return result;
}

/**
Converts the specified char hex letter to its integer equivalaent.
@param char c: The char hex letter you'd like to convert.
@return the integer code for it.
*/
int Converter::char2int(char c){
    if (c<='9'&&c>='0') return (int)c-(int)'0';
    if (c<='f'&&c>='a') return (int)c-(int)'a'+10;
    if (c<='F'&&c>='A') return (int)c-(int)'A'+10;
    return -1;
}
