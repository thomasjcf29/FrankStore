#include "../header_files/FrankThreader.h"
#include "../header_files/StaticFunctions.h"
#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

using namespace std;

FrankThreader::FrankThreader(MainScreen *mainScreen){
    for(int i = 0; i < numThreads; i++){
        pool.push_back(thread(&FrankThreader::loopFunction, this));
    }

    uiManager = mainScreen;
}

FrankThreader::~FrankThreader(){
    terminateAllThreads();
}

void FrankThreader::loopFunction(){
    while(true)
    {
        JobStruct job;
        bool empty = false;
        {
            unique_lock<mutex> lock(queueLock);

            condition.wait(lock, [this]{return !queueJobs.empty() || terminateAll;});

            if(terminateAll){
                return;
            }

            job = queueJobs.front();
            queueJobs.pop();
            empty = queueJobs.empty();
        }
        runJob(job);

        if(empty){
            uiManager->closeUIPopup();
        }
    }
};

void FrankThreader::terminateAllThreads(){
    {
        unique_lock<mutex> lock(queueLock);
        terminateAll = true;
    }
    condition.notify_all();
}

void FrankThreader::addJob(JobStruct job){
    {
        unique_lock<mutex> lock(queueLock);
        queueJobs.push(job);
    }
    condition.notify_one();
}

void FrankThreader::runJob(JobStruct job){

    uiManager->updateUIProgress(job.childNumber, InProgress);
    int result = 1;
    job.destFile = calculateFinalName(job);
    string overallCommand = calculateCommand(job);
    result = StaticFunctions::commandExecResult(overallCommand.c_str());

    if(job.action == FileEncryptAndEncode && result == 0){
        job.stageAt = Encrypted;
        job.sourceFile = job.destFile;
        job.destFile = calculateFinalName(job);
        overallCommand = calculateCommand(job);
        result = StaticFunctions::commandExecResult(overallCommand.c_str());

    }
    else if(job.action == FileDecodeAndDecrypt && result == 0){
        job.stageAt = Decoded;
        job.sourceFile = job.destFile;
        job.destFile = calculateFinalName(job);
        overallCommand = calculateCommand(job);
        result = StaticFunctions::commandExecResult(overallCommand.c_str());
    }

    if(result == 0){
        uiManager->updateUIProgress(job.childNumber, Success);
    }
    else{
        uiManager->updateUIProgress(job.childNumber, Error);
    }
}

string FrankThreader::calculateFinalName(JobStruct job){

    //It's decrypt so just remove the extension
    if(job.action == FileDecode || job.action == FileDecodeAndDecrypt){
        return removeFinalExtension(job.sourceFile);
    }

    if(job.action == FileEncryptAndEncode && job.stageAt == NotStarted){
        return job.sourceFile + ".enc";
    }

    if(job.outputImage){
        return job.sourceFile + ".png";
    }
    if(!job.outputImage){
        return job.sourceFile + ".bin";
    }

    return "";
}

string FrankThreader::removeFinalExtension(string fileName){
    size_t location = fileName.find_last_of(".");
    return fileName.substr(0, location);
}

string FrankThreader::calculateCommand(JobStruct job){
    stringstream command;
    bool displayExtraQuotes = false;

    if(job.fileName.empty()){
        command << "FrankStore ";
    }

    else{
        #ifdef WIN32
            command << "\"";
            displayExtraQuotes = true;
        #endif
        command << "\"" << job.fileName << "\" ";
    }

    if((job.action == FileEncryptAndEncode && job.stageAt == Encrypted) || (job.action == FileEncode && job.stageAt == NotStarted)){
        if(!job.outputImage){
            command << "encode \"" << job.imageCover << "\" \"" << job.sourceFile << "\" \"" << job.destFile << "\"" << " file";
        }
        else{
            command << "encode \"" << job.imageCover << "\" \"" << job.sourceFile << "\" \"" << job.destFile << "\"" << " image";
        }
    }
    else if(job.action == FileEncryptAndEncode && job.stageAt == NotStarted){
        command << "encrypt \"";

        command << job.sourceFile << "\" \"" << job.destFile << "\" ";

        if(job.passwordEncrypt.empty()){
            command << "image \"" << job.imageEncrypt << "\"";
        }
        else{
            command << "password \"" << job.passwordEncrypt << "\"";
            if(!job.imageEncrypt.empty()){
                command << " image \"" << job.imageEncrypt << "\"";
            }
        }
    }
    else if((job.action == FileDecodeAndDecrypt && job.stageAt == NotStarted) || (job.action == FileDecode && job.stageAt == NotStarted)){
        command << "decode \"" << job.imageCover << "\" \"" << job.sourceFile << "\" \"" << job.destFile << "\"";

        if(!job.outputImage){
            command << " file";
        }
        else{
            command << " image";
        }
    }
    else if(job.action == FileDecodeAndDecrypt && job.stageAt == Decoded){
        command << "decrypt \"";

        command << job.sourceFile << "\" \"" << job.destFile << "\" ";

        if(job.passwordEncrypt.empty()){
            command << "image \"" << job.imageEncrypt << "\"";
        }
        else{
            command << "password \"" << job.passwordEncrypt << "\"";
            if(!job.imageEncrypt.empty()){
                command << " image \"" << job.imageEncrypt << "\"";
            }
        }
    }

    command << " >> \"" << job.destFile << ".output\"";

    if(displayExtraQuotes){
        command << "\"";
    }

    return command.str();
}
