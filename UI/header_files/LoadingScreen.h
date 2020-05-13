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
