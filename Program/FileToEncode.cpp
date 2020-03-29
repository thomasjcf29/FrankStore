#include "FileToEncode.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

FileToEncode::FileToEncode(){

}

FileToEncode::FileToEncode(string file){
    inputFile = ifstream(file, ifstream::binary);

    if(!inputFile){
        valid = false;
        cout << "[ERROR]: Unable to open the file to encode, do you have permission?" << endl;
        return;
    }

    //Get Total To Read
    inputFile.seekg( 0, ios::end);
    totalLength = inputFile.tellg();

    cout << "Total length of file to encode (in bytes): " << totalLength << endl;

    inputFile.seekg(0, ios::beg);
}

bool FileToEncode::isValid(){
    return valid;
}

char* FileToEncode::getNextBytes(){
    size_t currentLocation = inputFile.tellg();

    if((totalLength - currentLocation) > 1024){
        bufferSize = 1024;
    }
    else{
        bufferSize = totalLength - currentLocation;
    }

    readSoFar += bufferSize;

    inputFile.read(buffer, bufferSize);

    if(inputFile.tellg() >= totalLength){
        isRead = true;
    }

    return buffer;
}

size_t FileToEncode::getBufferSize(){
    return bufferSize;
}

size_t FileToEncode::getTotalSize(){
    return totalLength;
}

size_t FileToEncode::getReadSoFar(){
    return readSoFar;
}

bool FileToEncode::isFileRead(){
    return isRead;
}

void FileToEncode::close(){
    inputFile.close();
}
