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

#ifndef FRANKSTOREUI_LOADINGSCREEN_H
#define FRANKSTOREUI_LOADINGSCREEN_H

#include <gtkmm.h>
#include <string>

class LoadingScreen{
private:
    Gtk::Window* pWindow = nullptr;
    Gtk::FileChooserDialog* pChooser = nullptr;
    Gtk::Button* dialogButton = nullptr;
    Gtk::Button* fileButton = nullptr;
    Gtk::Dialog* pDialog = nullptr;
    std::string fileName;
    bool valid = false;

    bool readyUp();
    bool foundFrankstore();
    void on_dialog_ok();
    void on_file_ok();
public:
    LoadingScreen();
    ~LoadingScreen();
    Gtk::Window* getWindow();
    bool isValid();
    std::string getExecutable();
};

#endif //FRANKSTOREUI_LOADINGSCREEN_H
