#include "FileToDecode.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

FileToDecode::FileToDecode(){

}

FileToDecode::FileToDecode(string name){
    fileName = name;
    inputFile = ifstream(fileName, ifstream::binary);

    if(!inputFile){
        valid = false;
        cout << "[ERROR]: Unable to open the file to encode, do you have permission?" << endl;
        return;
    }

    //Get Total To Read
    inputFile.seekg( 0, ios::end);
    totalBytes = inputFile.tellg();

    cout << "Total length of file to encode (in bytes): " << totalBytes << endl;

    if((totalBytes % 3) != 0){
        cout << "[ERROR]: Invalid decode file!" << endl;
        valid = false;
        return;
    }

    inputFile.seekg(0, ios::beg);
    read = false;
}

FileToDecode::~FileToDecode(){
}

bool FileToDecode::isValid(){
    return valid;
}

bool FileToDecode::isRead(){
    return read;
}

size_t FileToDecode::getBufferSize(){
    return bufferSize;
}

size_t FileToDecode::getTotalBytes(){
    return totalBytes;
}

size_t FileToDecode::getBytesRead(){
    return bytesReadSoFar;
}

Location* FileToDecode::getNextPixels(){
    if(((totalBytes / 3) - (bytesReadSoFar / 3)) > 1024){
        bufferSize = 1024;
    }
    else{
        bufferSize = (totalBytes / 3) - (bytesReadSoFar / 3);
    }

    bytesReadSoFar += bufferSize * 3;

    for(int i = 0; i < bufferSize; i++){
        Location loc;

        unsigned short details[3];

        for(int ii = 0; ii < 3; ii++){
            unsigned short temp;
            inputFile.read(reinterpret_cast<char *>(&temp), sizeof(unsigned short));
            cout << "Test: " << temp << endl;
            // int number = Converter::hex2int(hex);
            //
            // details[ii] = number;
        }

        loc = Location{details[0], details[1], details[2]};

        buffer[i] = loc;
    }

    if(bytesReadSoFar >= totalBytes){
        read = true;
    }

    return buffer;
}
