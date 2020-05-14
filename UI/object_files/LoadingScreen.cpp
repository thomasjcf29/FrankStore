/**
This file is part of FrankStore.

FrankStore is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

FrankStore is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with FrankStore.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../header_files/LoadingScreen.h"
#include "../header_files/StaticFunctions.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

/**
Constructor for the loading screen, openings the resource files and gathers all
relevant objects from the GUI. Do not use if isValid() is false. Also sets up
button listeners etc.
*/
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

    refBuilder->get_widget("fileChooser", pChooser);
    refBuilder->get_widget("alertBox", pDialog);
    refBuilder->get_widget("okButton", dialogButton);
    refBuilder->get_widget("fileButton", fileButton);

    if(!pDialog || !dialogButton || !fileButton || !pChooser){
        cerr << "Invalid glade file!" << endl;
        exit(1);
    }

    dialogButton->signal_clicked().connect(sigc::mem_fun(*this, &LoadingScreen::on_dialog_ok));
    fileButton->signal_clicked().connect(sigc::mem_fun(*this, &LoadingScreen::on_file_ok));

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

/**
Destructor for loading screen, used to delete any resources created by itself.
*/
LoadingScreen::~LoadingScreen(){
    delete pWindow;
}

/**
@return the pWindow object which can be displayed by the Application.
*/
Gtk::Window* LoadingScreen::getWindow(){
    return pWindow;
}

/**
@return if the object is ok to use (true if ok, false if not).
*/
bool LoadingScreen::isValid(){
    return valid;
}

/**
@return the string executable location as selected in the file chooser.
*/
string LoadingScreen::getExecutable(){
    return fileName;
}

/**
Called by the 5 second timer to check if location exists and display dialog etc.
@return false to prevent being called again.
*/
bool LoadingScreen::readyUp(){

    if(!foundFrankstore()){
        pDialog->run();
        pChooser->run();
    }

    pWindow->close();

    return false;
}

/**
Function called when ok button is pressed in dialog.
*/
void LoadingScreen::on_dialog_ok(){
    pDialog->hide();
}

/**
Function called when open button is pressed in window chooser.
*/
void LoadingScreen::on_file_ok(){
    fileName = pChooser->get_filename();
    struct stat info;

    if(stat(fileName.c_str(), &info) != 0){
        return;
    }
    else if(info.st_mode & S_IFDIR){
        pChooser->set_current_folder(fileName);
        return;
    }
    else{
        pChooser->hide();
    }
}

/**
@return if FrankStore could be found in the system environment.
*/
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

        if(!localResult.empty()){
            fileName = "./FrankStore";
        }

        return true;

    #endif
}
