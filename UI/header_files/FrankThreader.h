#ifndef FRANKSTOREUI_FRANKTHREADER_H
#define FRANKSTOREUI_FRANKTHREADER_H

#include "MainScreen.h"
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

#ifndef FRANKSTOREUI_UPDATEMESSAGE_ENUM
#define FRANKSTOREUI_UPDATEMESSAGE_ENUM

enum UpdateMessage {Error, InProgress, Success};

#endif //FRANKSTOREUI_UPDATEMESSAGE_ENUM

#ifndef FRANKSTOREUI_STAGE_ENUM
#define FRANKSTOREUI_STAGE_ENUM

enum FileStage {NotStarted, Encrypted, Encoded, Decoded, Decrypted};

#endif //FRANKSTOREUI_STAGE_ENUM

#ifndef FRANKSTOREUI_ACTION_ENUM
#define FRANKSTOREUI_ACTION_ENUM

enum FileAction {FileEncode, FileEncryptAndEncode, FileDecode, FileDecodeAndDecrypt};

#endif //FRANKSTOREUI_ACTION_ENUM

#ifndef FRANKSTOREUI_JOBSTRUCT
#define FRANKSTOREUI_JOBSTRUCT

struct JobStruct{
    std::string fileName, sourceFile, destFile, imageCover, imageEncrypt, passwordEncrypt;
    int childNumber;
    bool outputImage;
    FileStage stageAt;
    FileAction action;
};

#endif //FRANKSTOREUI_JOBSTRUCT

class MainScreen;

class FrankThreader{
private:
    MainScreen *uiManager = nullptr;
    int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> pool;
    std::mutex queueLock;
    std::condition_variable condition;
    bool terminateAll = false;
    std::queue<JobStruct> queueJobs;

    void terminateAllThreads();
    void runJob(JobStruct job);
    FrankThreader();
    std::string calculateFinalName(JobStruct job);
    std::string removeFinalExtension(std::string fileName);
    std::string calculateCommand(JobStruct job);
public:
    FrankThreader(MainScreen *mainScreen);
    ~FrankThreader();
    void loopFunction();
    void addJob(JobStruct job);
};

#endif //FRANKSTOREUI_FRANKTHREADER_H
