#include "../header_files/LoadingScreen.h"
#include "../header_files/StaticFunctions.h"
#include <iostream>

using namespace std;

LoadingScreen::LoadingScreen(){
    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_resource("/loading_screen/designs/loading_screen.glade");
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

    css_provider->load_from_resource("/loading_screen/designs/application.css");

    refBuilder->get_widget("loadingScreen", pWindow);
    if(pWindow){
        pWindow->show_all();
        pWindow->show_all_children();
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        int timeout_value = 5000; //5s
        sigc::slot<bool> my_slot = sigc::mem_fun(*this, &LoadingScreen::readyUp);
        Glib::signal_timeout().connect(my_slot, timeout_value);
        valid = true;
    }
}

LoadingScreen::~LoadingScreen(){
    delete pWindow;
}

bool LoadingScreen::readyUp(){
    string result = StaticFunctions::commandExec("echo hello");
    cout << "[TEST]: " << result << endl;
    return true;
}

Gtk::Window* LoadingScreen::getWindow(){
    return pWindow;
}

bool LoadingScreen::isValid(){
    return valid;
}
