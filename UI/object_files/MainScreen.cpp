#include "../header_files/MainScreen.h"
#include <iostream>
#include <string>

using namespace std;

MainScreen::MainScreen(string application){
    executableLocation = application;

    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_resource("/main_screen/designs/main_screen.glade");
    }
    catch(const Glib::FileError& ex){
        cerr << "FileError: " << ex.what() << endl;
        return;
    }
    catch(const Glib::MarkupError& ex)
    {
        cerr << "MarkupError: " << ex.what() << endl;
        return;
    }
    catch(const Gtk::BuilderError& ex)
    {
        cerr << "BuilderError: " << ex.what() << endl;
        return;
    }

    auto css_provider = Gtk::CssProvider::create();

    css_provider->signal_parsing_error().connect(
      [](const Glib::RefPtr<const Gtk::CssSection>& section,
         const Glib::Error& error) {
        cerr << "CSS Error: " << error.what() << endl;
        exit(1);
      });

    css_provider->load_from_resource("/main_screen/designs/application.css");

    //Windows / Dialogs
    refBuilder->get_widget("mainWindow", pWindow);
    refBuilder->get_widget("gridEncryption", gridEncryption);

    //Encryption Section
    refBuilder->get_widget("btnAddImageKey", btnAddImageKey);
    refBuilder->get_widget("btnEditImageKey", btnEditImageKey);
    refBuilder->get_widget("btnDelImageKey", btnDelImageKey);
    refBuilder->get_widget("btnAddPassword", btnAddPassword);
    refBuilder->get_widget("btnEditPassword", btnEditPassword);
    refBuilder->get_widget("btnDelPassword", btnDelPassword);
    refBuilder->get_widget("switchEncryption", switchEncryption);

    if(!btnAddImageKey || !btnEditImageKey || !btnDelImageKey || !btnAddPassword || !btnEditPassword || !btnDelPassword || !switchEncryption){
        cout << "Invalid glade file!" << endl;
        exit(1);
    }

    switchEncryption->signal_button_press_event().connect(sigc::mem_fun(*this, &MainScreen::switch_encryption_pressed));

    if(pWindow){
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        valid = true;
    }

}

MainScreen::~MainScreen(){
    delete pWindow;
    delete btnAddImageKey;
    delete btnEditImageKey;
    delete btnDelImageKey;
    delete btnAddPassword;
    delete btnEditPassword;
    delete btnDelPassword;
    delete switchEncryption;
}

Gtk::Window* MainScreen::getWindow(){
    return pWindow;
}

bool MainScreen::isValid(){
    return valid;
}

void MainScreen::switch_encryption_pressed(){
    encrypt = !encrypt;

    if(encrypt){
        gridEncryption->show();
    }
    else{
        gridEncryption->hide();
    }
}
