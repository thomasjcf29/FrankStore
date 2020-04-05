#include "FileToDecode.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/**
A blank constructor for the object, this is only to be used to allow it to be
referenced in a header file. Do not use this object based on this.
*/
FileToDecode::FileToDecode(){

}

/**
Returns an object to handle the decoding of a file. Check the object is valid
before using it (.isValid()).
@param strign name: The file location name.
*/
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

    if((totalBytes % 2) != 0){
        cout << "[ERROR]: Invalid decode file!" << endl;
        valid = false;
        return;
    }

    totalActualBytes = totalBytes / 2;

    if((totalActualBytes % 3) != 0){
        cout << "[ERROR]: Invalid decode file!" << endl;
        valid = false;
        return;
    }

    inputFile.seekg(0, ios::beg);
    valid = true;
    read = false;
}

/**
@return if the object is valid: true if valid, false if not.
*/
bool FileToDecode::isValid(){
    return valid;
}

/**
@return if the file is read: true if read, false if not.
*/
bool FileToDecode::isRead(){
    return read;
}

/**
@return the size of the buffer used during the last file read. Can be between
0-1024.
*/
size_t FileToDecode::getBufferSize(){
    return bufferSize;
}

/**
@return the total bytes of the input file. For usable bytes divide by 2 as there
are two bytes per a number!
*/
size_t FileToDecode::getTotalBytes(){
    return totalBytes;
}

/**
@return the total amount of bytes read so far.
*/
size_t FileToDecode::getBytesRead(){
    return bytesReadSoFar;
}

/**
@return the location of the next amount of read bytes from the file. Can be between
0-1024, depending on how bytes are left.
*/
Location* FileToDecode::getNextPixels(){
    if(((totalActualBytes / 3) - (bytesReadSoFar / 3)) > 1024){
        bufferSize = 1024;
    }
    else{
        bufferSize = (totalActualBytes / 3) - (bytesReadSoFar / 3);
    }

    bytesReadSoFar += bufferSize * 3;

    for(int i = 0; i < bufferSize; i++){
        Location loc;

        unsigned short details[3];

        for(int ii = 0; ii < 3; ii++){
            unsigned short temp;
            inputFile.read(reinterpret_cast<char *>(&temp), sizeof(unsigned short));
            details[ii] = temp;
        }

        loc = Location{details[0], details[1], details[2]};

        buffer[i] = loc;
    }

    if(bytesReadSoFar >= totalActualBytes){
        read = true;
    }

    return buffer;
}

/**
Close the reference to the file.
*/
void FileToDecode::close(){
    inputFile.close();
}
