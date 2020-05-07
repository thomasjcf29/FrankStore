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

    refBuilder->get_widget("mainWindow", pWindow);
    if(pWindow){
        pWindow->show_all();
        pWindow->show_all_children();
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        valid = true;
    }

}

MainScreen::~MainScreen(){
    delete pWindow;
}

Gtk::Window* MainScreen::getWindow(){
    return pWindow;
}

bool MainScreen::isValid(){
    return valid;
}
