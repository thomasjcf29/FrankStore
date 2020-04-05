#ifndef FRANKSTORE_FILETODECODE_H
#define FRANKSTORE_FILETODECODE_H

#ifndef FRANKSTORE_LOCATION
#define FRANKSTORE_LOCATION
    struct Location{
        int x, y, hash;
    };
#endif //FRANKSTORE_LOCATION

#include <fstream>
#include <string>

class FileToDecode{
private:
    std::ifstream inputFile;
    std::string fileName;
    bool read;
    bool valid;
    size_t totalBytes = 0;
    size_t totalActualBytes = 0;
    size_t bytesReadSoFar = 0;
    size_t bufferSize = 0;
    Location buffer[1024];
public:
    FileToDecode();
    FileToDecode(std::string name);
    bool isValid();
    bool isRead();
    size_t getBufferSize();
    size_t getTotalBytes();
    size_t getBytesRead();
    Location* getNextPixels();
    void close();
};

#endif //FRANKSTORE_FILETODECODE_H
