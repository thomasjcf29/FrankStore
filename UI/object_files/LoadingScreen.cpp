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

    refBuilder->get_widget("loadingScreen", pWindow);
    if(pWindow){
        pWindow->show_all();
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
