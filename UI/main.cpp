#include "header_files/helloworld.h"
#include "header_files/LoadingScreen.h"
#include <iostream>
#include <gtkmm.h>

using namespace std;

int main(int argc, char **argv){
    cout << "Hello World!" << endl;

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    HelloWorld helloworld;
    LoadingScreen loadingScreen;

    return app->run(helloworld);
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
