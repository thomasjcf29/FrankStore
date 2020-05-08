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
    Gtk::Paned* pneController = nullptr;
    Gtk::Dialog* passwordDialog = nullptr;

    //Encryption Section
    Gtk::Button* btnAddImageKey = nullptr;
    Gtk::Button* btnEditImageKey = nullptr;
    Gtk::Button* btnDelImageKey = nullptr;
    Gtk::Button* btnAddPassword = nullptr;
    Gtk::Button* btnEditPassword = nullptr;
    Gtk::Button* btnDelPassword = nullptr;
    Gtk::CheckButton* chkEncryption = nullptr;

    //Password Section
    Gtk::Button* btnPasswordChosen = nullptr;
    Gtk::Entry* passwordEntry = nullptr;
    Gtk::Label* errorLabel = nullptr;

    void checkbox_encryption_toggled();
    void btn_add_image();
    void btn_edit_image();
    void btn_del_image();
    void btn_add_pwd();
    void btn_edit_pwd();
    void btn_del_pwd();

    void btn_pwd_chosen();

    void show_encryption_parts();
public:
    MainScreen(std::string application);
    ~MainScreen();
    Gtk::Window* getWindow();
    bool isValid();
};

#endif //FRANKSTOREUI_MAINSCREEN_H
