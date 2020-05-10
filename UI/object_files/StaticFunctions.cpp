#include "../header_files/StaticFunctions.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

string StaticFunctions::commandExec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int StaticFunctions::commandExecResult(const char* cmd) {
    auto pipe = popen(cmd, "r"); // get rid of shared_ptr
    auto rc = pclose(pipe);

    if(rc == EXIT_SUCCESS){ // == 0
        return 0;
    }
    else{
        return 1;
    }
}
