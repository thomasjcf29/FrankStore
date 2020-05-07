#ifndef FRANKSTOREUI_MAINSCREEN_H
#define FRANKSTOREUI_MAINSCREEN_H

#include <gtkmm.h>
#include <string>

class MainScreen{
private:
    std::string executableLocation, coverImage, encryptPassword, encryptImage;
    bool valid = false;
    bool encrypt = false;

    //Windows / Dialogs
    Gtk::Window* pWindow = nullptr;
    Gtk::Grid* gridEncryption = nullptr;

    //Encryption Section
    Gtk::Button* btnAddImageKey = nullptr;
    Gtk::Button* btnEditImageKey = nullptr;
    Gtk::Button* btnDelImageKey = nullptr;
    Gtk::Button* btnAddPassword = nullptr;
    Gtk::Button* btnEditPassword = nullptr;
    Gtk::Button* btnDelPassword = nullptr;
    Gtk::Switch* switchEncryption = nullptr;
    

public:
    MainScreen(std::string application);
    ~MainScreen();
    Gtk::Window* getWindow();
    bool isValid();
};

#endif //FRANKSTOREUI_MAINSCREEN_H
