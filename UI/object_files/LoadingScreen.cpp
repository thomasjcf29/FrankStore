#include "../header_files/LoadingScreen.h"
#include <iostream>

LoadingScreen::LoadingScreen(){
    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_resource("/loading_screen/designs/loading_screen.glade");
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

    css_provider->signal_parsing_error().connect(
      [](const Glib::RefPtr<const Gtk::CssSection>& section,
         const Glib::Error& error) {
        std::cerr << "CSS Error: " << error.what() << std::endl;
        std::exit(1);
      });

    css_provider->load_from_resource("/loading_screen/designs/application.css");

    refBuilder->get_widget("loadingScreen", pWindow);
    if(pWindow){
        pWindow->show_all();
        refBuilder->get_widget("loadingSpinner", pSpinner);
        if(!pSpinner){
            std::cerr << "Error loading glade design." << std::endl;
            return;
        }
        pSpinner->start();
        pSpinner->show();
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        valid = true;
    }
}

LoadingScreen::~LoadingScreen(){
    delete pWindow;
    delete pSpinner;
}

Gtk::Window* LoadingScreen::getWindow(){
    return pWindow;
}

bool LoadingScreen::isValid(){
    return valid;
}
