#include "FileToOutput.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

FileToOutput::FileToOutput(){

}

FileToOutput::FileToOutput(string file){
    fileName = file;
    outputFile.open(file, ifstream::binary);

    if(!outputFile){
        valid = false;
        cout << "[ERROR]: Unable to create the output file, do you have permission?" << endl;
        return;
    }
}

void FileToOutput::write(ushort *data){
    size_t size = sizeof(data);
    outputFile.write((char*)data, size);
}

bool FileToOutput::isValid(){
    return valid;
}

void FileToOutput::close(){
    outputFile.close();
}
