#include "header_files/LoadingScreen.h"
#include "header_files/FileChooser.h"
#include <iostream>
#include <gtkmm.h>

using namespace std;

int main(int argc, char **argv){
    auto app = Gtk::Application::create(argc, argv, "uk.co.thomasfranklin.frankstore");

    // LoadingScreen loadingScreen;
    //
    // if(loadingScreen.isValid()){
    //     app->run(*(loadingScreen.getWindow()));
    // }

    FileChooser loadingScreen;

    if(loadingScreen.isValid()){
        app->run(*(loadingScreen.getFileChooser()));
    }

    return 0;
}
