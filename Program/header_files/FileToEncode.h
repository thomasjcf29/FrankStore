#ifndef FRANKSTORE_FILETOENCODE_H
#define FRANKSTORE_FILETOENCODE_H

#include <fstream>
#include <string>

class FileToEncode{
private:
    std::ifstream inputFile;
    bool isRead = false;
    bool valid = true;
    char buffer[1024];
    size_t totalLength;
    size_t bufferSize;
    size_t readSoFar = 0;
public:
    FileToEncode();
    FileToEncode(std::string file);
    char* getNextBytes();
    bool isFileRead();
    bool isValid();
    size_t getBufferSize();
    size_t getTotalSize();
    size_t getReadSoFar();
    void close();
};

#endif //FRANKSTORE_FILETOENCODE_H
