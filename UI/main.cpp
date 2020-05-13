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
