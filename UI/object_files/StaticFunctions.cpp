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

#include "../header_files/StaticFunctions.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

/**
Static function to return the response of a command execute on the system enviornment.
@param const char* cmd: The command you would like to run.
*/
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

/**
Static function to return if the command executed successfully, 0 if true, 1 if false.
Will return 1 no matter what the exit code of the actual application is. If you do not
use a way to write the output to file, it will be displayed in the console!
@param const char* cmd: The command you would like to run.
*/
int StaticFunctions::commandExecResult(const char* cmd) {
    int result = system(cmd);

    if(result == 0){ // == 0
        return 0;
    }
    else{
        return 1;
    }
}
