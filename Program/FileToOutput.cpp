#include "FileToOutput.h"
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
    outputFile.open(file, ios::binary);

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

int FileToOutput::cal(char c)// cal the coresponding value in hex of ascii char c
{
    if (c<='9'&&c>='0') return (int)c-(int)'0';
    if (c<='f'&&c>='a') return (int)c-(int)'a'+10;
    if (c<='F'&&c>='A') return (int)c-(int)'A'+10;
}

void FileToOutput::writetest(const char* data){
    size_t size = strlen(data);
    for(int i = 0; i < size; i+=2){
        char letters = cal(data[i])*16+cal(data[i+1]);
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
