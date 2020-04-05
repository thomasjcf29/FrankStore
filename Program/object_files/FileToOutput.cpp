#include "../header_files/Converter.h"
#include "../header_files/FileToOutput.h"
#include <string>
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
FileToOutput::FileToOutput(){

}

/**
Creates the object and creates the output file.
@param string file: The file you wish to output to.
*/
FileToOutput::FileToOutput(string file){
    fileName = file;
    outputFile = ofstream(file, ofstream::binary);

    if(!outputFile){
        valid = false;
        cout << "[ERROR]: Unable to create the output file, do you have permission?" << endl;
        return;
    }
}

/**
Writes the bytes passed in to the file.
@param unsigned short data: The bytes you would like to ouput.
*/
void FileToOutput::write(unsigned short data){
    outputFile.write(reinterpret_cast<const char*>(&data), sizeof(unsigned short));
}

/**
Writes the char array provided to the output file as binary.
@param const char* data: The char data of hex letters you would like written.
Pass in hex letters in there character ('A') form, this will automatically
convert to integer.
*/
void FileToOutput::writetest(const char* data){
    size_t size = strlen(data);
    for(int i = 0; i < size; i+=2){
        char letters = Converter::char2int(data[i])*16+Converter::char2int(data[i+1]);
        outputFile << letters;
    }
}

/**
@return if the object is valid, do not use this object if it is false.
This will be false if the file cannot be read.
*/
bool FileToOutput::isValid(){
    return valid;
}

/**
Flush the buffer to the file, should be done to ensure the buffer is not too high.
*/
void FileToOutput::flush(){
    outputFile.flush();
}

/**
Call this method when you have finished writing, it closes the file handler.
*/
void FileToOutput::close(){
    outputFile.close();
}
