#include "../header_files/LoadingScreen.h"
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <iostream>


using namespace std;

LoadingScreen::LoadingScreen(){
    if(ui){
        cout << "UI True" << endl;
    }
    else{
        cout << "UI False" << endl;
    }
}

LoadingScreen::~LoadingScreen(){

}
