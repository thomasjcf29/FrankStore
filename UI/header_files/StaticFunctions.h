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

#ifndef FRANKSTOREUI_STATICFUNCTIONS_H
#define FRANKSTOREUI_STATICFUNCTIONS_H

#include <string>

class StaticFunctions{
private:
    StaticFunctions();
public:
    static std::string commandExec(const char* cmd);
    static int commandExecResult(const char* cmd);
};

#endif //FRANKSTOREUI_STATICFUNCTIONS_H
