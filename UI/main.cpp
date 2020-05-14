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

#include "header_files/LoadingScreen.h"
#include "header_files/MainScreen.h"
#include <iostream>
#include <gtkmm.h>
#include <string>

using namespace std;

/**
Entrance point to the program, called by the user.
Creates the loading screen and then creates the main screen.
Returns exit code of 0 (success) or 1 (failed).
*/
int main(int argc, char **argv){
    auto app = Gtk::Application::create(argc, argv, "uk.co.thomasfranklin.frankstoreui.loadingScreen");

    LoadingScreen loadingScreen;
    string applicationLocation;

    if(!loadingScreen.isValid()){
        cout << "[ERROR]: Invalid application startup." << endl;
        return 1;
    }

    app->run(*(loadingScreen.getWindow()));
    applicationLocation = loadingScreen.getExecutable();

    app = Gtk::Application::create(argc, argv, "uk.co.thomasfranklin.frankstoreui.mainScreen");

    MainScreen mainScreen(applicationLocation);

    if(!mainScreen.isValid()){
        cout << "[ERROR]: Invalid main application startup." << endl;
        return 1;
    }

    app->run(*(mainScreen.getWindow()));
}
