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
public:
    FileToEncode();
    FileToEncode(std::string file);
    char* getNextBytes();
    bool isFileRead();
    bool isValid();
    size_t getBufferSize();
    void close();
};

#endif //FRANKSTORE_FILETOENCODE_H
