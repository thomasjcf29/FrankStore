#include "../header_files/FileToEncode.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

/**
Blank constructor for the class, used to allow headers to point to it.
Do not use the object if initialised with this constructor!
*/
FileToEncode::FileToEncode(){

}

/**
Constructor used to initialise the object, open the file, and set object variables.
@param string file: The file you would like to read.
*/
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

/**
@return if the object is valid. This will be based on whether the file could be read.
If false, do not use this object!
*/
bool FileToEncode::isValid(){
    return valid;
}

/**
@return a pointer to the char array containing the next block of bytes.
This will be at maximum 1024 bytes, or whatever is left of the file.
You can check how many bytes by calling getBufferSize() after you've called
this method.
*/
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

/**
@return the buffersize of the last amount read.
*/
size_t FileToEncode::getBufferSize(){
    return bufferSize;
}

/**
@return the total filesize in bytes.
*/
size_t FileToEncode::getTotalSize(){
    return totalLength;
}

/**
@return the total amount of bytes read so far.
*/
size_t FileToEncode::getReadSoFar(){
    return readSoFar;
}

/**
@return if the file has been completely read.
*/
bool FileToEncode::isFileRead(){
    return isRead;
}

/**
When the object is not required call this and it will release all resources.
*/
void FileToEncode::close(){
    inputFile.close();
}
