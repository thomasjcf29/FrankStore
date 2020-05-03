#include "header_files/helloworld.h"
#include <iostream>
#include <gtkmm.h>

using namespace std;

int main(int argc, char **argv){
    cout << "Hello World!" << endl;

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    HelloWorld helloworld;

    return app->run(helloworld);
}
