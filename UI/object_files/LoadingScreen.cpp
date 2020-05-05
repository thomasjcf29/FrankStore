#include "../header_files/LoadingScreen.h"
#include <iostream>

LoadingScreen::LoadingScreen(){
    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_file("designs/loading_screen.glade");
    }
    catch(const Glib::FileError& ex){
        std::cerr << "FileError: " << ex.what() << std::endl;
        return;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return;
    }

    auto css_provider = Gtk::CssProvider::create();
    css_provider->load_from_resource("designs/application.css");

    if(not css_provider){
        std::cerr << "Cannot load CSS file" << std::endl;
        return;
    }

    refBuilder->get_widget("loadingScreen", pWindow);
    if(pWindow){
        pWindow->show_all();
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        valid = true;
    }
}

LoadingScreen::~LoadingScreen(){
    delete pWindow;
}

Gtk::Window* LoadingScreen::getWindow(){
    return pWindow;
}

bool LoadingScreen::isValid(){
    return valid;
}
