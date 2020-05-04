#include "../header_files/LoadingScreen.h"
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <iostream>


using namespace std;

LoadingScreen::LoadingScreen() : ui{Gtk::Builder::create_from_file("designs\\loading_screen.glade")}{
    if(ui){
        cout << "UI True" << end;
    }
    else{
        cout << "UI True" << end;
    }
}
