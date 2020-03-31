#include "FileToOutput.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

FileToOutput::FileToOutput(){

}

FileToOutput::FileToOutput(string file){
    fileName = file;
    outputFile.open(file);

    if(!outputFile){
        valid = false;
        cout << "[ERROR]: Unable to create the output file, do you have permission?" << endl;
        return;
    }
}

void FileToOutput::write(unsigned short data){
    outputFile.write(reinterpret_cast<const char*>(&data), sizeof(unsigned short));
}

bool FileToOutput::isValid(){
    return valid;
}

void FileToOutput::flush(){
    outputFile.flush();
}

void FileToOutput::close(){
    outputFile.close();
}
