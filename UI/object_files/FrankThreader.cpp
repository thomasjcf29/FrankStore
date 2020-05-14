/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../header_files/FrankThreader.h"
#include "../header_files/StaticFunctions.h"
#include <string>
#include <iostream>
#include <mutex>
#include <sstream>

using namespace std;

/**
Constructor for FrankThreader (Thread Manager). It automatically creates
max amount of threads allowed for the system and places them into hold.
When you add jobs via addJob() the threads are one by one kicked off.
@param MainScreen *mainScreen: The parent GUI manager, used to update GUI progress.
*/
FrankThreader::FrankThreader(MainScreen *mainScreen){
    for(int i = 0; i < numThreads; i++){
        pool.push_back(thread(&FrankThreader::loopFunction, this));
    }

    uiManager = mainScreen;
}

/**
Destructor for the thread manager, safely terminates all threads.
*/
FrankThreader::~FrankThreader(){
    terminateAllThreads();
}

/**
Threads internal loop system, thread will continually loop until quit (when terminateAll) is called.
If no jobs thread will pause until told to run.
*/
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
        }
        runJob(job);

        {
            unique_lock<mutex> lock(queueLock);
            empty = queueJobs.empty();
        }

        if(empty){
            uiManager->closeUIPopup();
        }
    }
};

/**
Safely terminates all threads within the thread manager.
*/
void FrankThreader::terminateAllThreads(){
    {
        unique_lock<mutex> lock(queueLock);
        terminateAll = true;
    }
    condition.notify_all();
}

/**
Publically add job to the list of jobs waiting to run.
@param JobStruct job: The job you would like to run.
*/
void FrankThreader::addJob(JobStruct job){
    {
        unique_lock<mutex> lock(queueLock);
        queueJobs.push(job);
    }
    condition.notify_one();
}

/**
Internal to thread, take the JobStruct, work out the necessary command and process as required.
@param JobStruct job: The job to process.
*/
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

/**
Calcalate the destination fileName for the job.
@param JobStruct job: The job to calculate the name for.
@return string: The final fileName.
*/
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

/**
Remove the final extension from the file (decoding).
@param string: The fileName to remove the extension from.
@return string: The file with final extension removed.
*/
string FrankThreader::removeFinalExtension(string fileName){
    size_t location = fileName.find_last_of(".");
    return fileName.substr(0, location);
}

/**
Calculate the command for the Job, depending on the command and passed in params determines the commnad on the command line.
@param JobStruct job: The job to calculate the command for.
@return string the command to run in the executor.
*/
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

    command << " > \"" << job.destFile << ".output\"";

    if(displayExtraQuotes){
        command << "\"";
    }

    return command.str();
}
