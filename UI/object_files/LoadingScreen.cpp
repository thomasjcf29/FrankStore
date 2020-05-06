#include "../header_files/LoadingScreen.h"
#include "../header_files/StaticFunctions.h"
#include "../header_files/FileChooser.h"
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

Gtk::Window* LoadingScreen::getWindow(){
    return pWindow;
}

bool LoadingScreen::isValid(){
    return valid;
}

bool LoadingScreen::readyUp(){
    string result = StaticFunctions::commandExec("echo %cd%");
    cout << "[FOUND]: " << foundFrankstore() << endl;

    if(!foundFrankstore()){
        FileChooser fileCh;
        if(fileCh.isValid()){
            auto app = pWindow->get_application();
            app->add_window(*fileCh.getFileChooser());
        }
    }
    return false;
}

bool LoadingScreen::foundFrankstore(){

    #ifdef WIN32

        //In Windows where returns a location if it's in the curent folder or
        //in the environment path
        string result = StaticFunctions::commandExec("where FrankStore /q && if %ERRORLEVEL% EQU 0 echo OK");

        //There will not be an OK if the file does not exist
        if(result.empty()){
            return false;
        }

        return true;

    #else

        //In Linux which will only return a result if it's in the environment
        //path, you will also have to check the local folder.
        string pathResult = StaticFunctions::commandExec("which FrankStore");
        string localResult = StaticFunctions::commandExec("ls -lA . | grep -ow FrankStore");

        if(pathResult.empty() && localResult.empty()){
            return false;
        }

        return true;

    #endif
}
