#ifndef FRANKSTORE_FILETOOUTPUT_H
#define FRANKSTORE_FILETOOUTPUT_H

#include <string>
#include <fstream>

class FileToOutput{
private:
    std::string fileName;
    std::ofstream outputFile;
    bool valid = true;
public:
    FileToOutput();
    FileToOutput(std::string fileName);
    bool isValid();
    void write(ushort *data);
    void close();
};

#endif //FRANKSTORE_FILETOOUTPUT_H
