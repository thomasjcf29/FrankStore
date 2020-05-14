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

#ifndef FRANKSTORE_CONSOLECONTROLLER_H
#define FRANKSTORE_CONSOLECONTROLLER_H

class ConsoleController{
public:
    static void SetStdinEcho(bool enable);
private:
    ConsoleController() {}
};

#endif //FRANKSTORE_CONSOLECONTROLLER_H
