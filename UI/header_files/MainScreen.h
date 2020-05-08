#ifndef FRANKSTOREUI_MAINSCREEN_H
#define FRANKSTOREUI_MAINSCREEN_H

#include <gtkmm.h>
#include <string>
#include <vector>

class MainScreen{
private:
    std::string executableLocation, coverImage, encryptPassword, encryptImage;
    bool valid = false;
    bool encrypt = false;
    bool choosingEncryptImage = false;
    bool choosingCoverImage = false;
    bool confirmFolder = false;
    vector<std::string> files;

    //Windows / Dialogs
    Gtk::Window* pWindow = nullptr;
    Gtk::Grid* gridEncryption = nullptr;
    Gtk::Paned* pneController = nullptr;
    Gtk::Dialog* passwordDialog = nullptr;
    Gtk::FileChooserDialog* encryptionImageChooser = nullptr;
    Gtk::Dialog* confirmationDialog = nullptr;
    Gtk::FileChooserDialog* filesToHideChooser = nullptr;

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

    //Encryption Image Section
    Gtk::Button* btnOpenEncryptionImage = nullptr;

    //Cover Image Selection
    Gtk::Button* btnCoverImage = nullptr;

    //File Addition
    Gtk::Button* btnFilesChosen = nullptr;
    Gtk::Button* btnFolderNo = nullptr;
    Gtk::Button* btnFolderYes = nullptr;
    Gtk::Button* btnAddFiles = nullptr;

    void checkbox_encryption_toggled();
    void btn_add_image();
    void btn_edit_image();
    void btn_del_image();
    void btn_add_pwd();
    void btn_edit_pwd();
    void btn_del_pwd();

    void btn_pwd_chosen();

    void btn_enc_image_chosen();

    void btn_sel_cover_image();

    void show_encryption_parts();

    void show_file_chooser();
    void btn_file_choosen();
    void btn_folder_no();
    void btn_folder_yes();
    
    void add_files(std::string folder);
public:
    MainScreen(std::string application);
    ~MainScreen();
    Gtk::Window* getWindow();
    bool isValid();
};

#endif //FRANKSTOREUI_MAINSCREEN_H
