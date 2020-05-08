#include "../header_files/MainScreen.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

MainScreen::MainScreen(string application){
    executableLocation = application;

    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_resource("/main_screen/designs/main_screen.glade");
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

    css_provider->load_from_resource("/main_screen/designs/application.css");

    //Windows / Dialogs
    refBuilder->get_widget("mainWindow", pWindow);
    refBuilder->get_widget("gridEncryption", gridEncryption);
    refBuilder->get_widget("pneController", pneController);
    refBuilder->get_widget("passwordDialog", passwordDialog);
    refBuilder->get_widget("encryptionImageChooser", encryptionImageChooser);

    //Encryption Section
    refBuilder->get_widget("btnAddImageKey", btnAddImageKey);
    refBuilder->get_widget("btnEditImageKey", btnEditImageKey);
    refBuilder->get_widget("btnDelImageKey", btnDelImageKey);
    refBuilder->get_widget("btnAddPassword", btnAddPassword);
    refBuilder->get_widget("btnEditPassword", btnEditPassword);
    refBuilder->get_widget("btnDelPassword", btnDelPassword);
    refBuilder->get_widget("chkEncryption", chkEncryption);

    //Password Section
    refBuilder->get_widget("btnPasswordChosen", btnPasswordChosen);
    refBuilder->get_widget("passwordEntry", passwordEntry);
    refBuilder->get_widget("errorLabel", errorLabel);

    //Encryption Image Section
    refBuilder->get_widget("btnOpenEncryptionImage", btnOpenEncryptionImage);

    if(!btnAddImageKey || !btnEditImageKey || !btnDelImageKey || !btnAddPassword ||
       !btnEditPassword || !btnDelPassword || !chkEncryption || !pneController ||
       !passwordDialog || !btnPasswordChosen || !passwordEntry || !errorLabel ||
       !encryptionImageChooser || !btnOpenEncryptionImage){
        cout << "Invalid glade file!" << endl;
        exit(1);
    }

    chkEncryption->signal_toggled().connect(sigc::mem_fun(*this, &MainScreen::checkbox_encryption_toggled));
    btnAddImageKey->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_add_image));
    btnEditImageKey->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_edit_image));
    btnDelImageKey->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_del_image));
    btnAddPassword->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_add_pwd));
    btnEditPassword->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_edit_pwd));
    btnDelPassword->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_del_pwd));

    btnPasswordChosen->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_pwd_chosen));

    btnOpenEncryptionImage->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_enc_image_chosen));

    if(pWindow){
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        pWindow->set_size_request(900, 440);
        pneController->set_position(550);
        valid = true;
    }
}

MainScreen::~MainScreen(){
    delete pWindow;
    delete btnAddImageKey;
    delete btnEditImageKey;
    delete btnDelImageKey;
    delete btnAddPassword;
    delete btnEditPassword;
    delete btnDelPassword;
    delete chkEncryption;
    delete pneController;
    delete passwordDialog;
    delete btnPasswordChosen;
    delete passwordEntry;
    delete errorLabel;
    delete encryptionImageChooser;
    delete btnOpenEncryptionImage;
}

Gtk::Window* MainScreen::getWindow(){
    return pWindow;
}

bool MainScreen::isValid(){
    return valid;
}

void MainScreen::checkbox_encryption_toggled(){
    encrypt = !encrypt;
    show_encryption_parts();
}

void MainScreen::btn_add_image(){
    encryptionImageChooser->run();
    show_encryption_parts();
}

void MainScreen::btn_edit_image(){
    encryptionImageChooser->run();
    show_encryption_parts();
}

void MainScreen::btn_del_image(){
    encryptImage.clear();
    show_encryption_parts();
}

void MainScreen::btn_add_pwd(){
    passwordDialog->run();
    show_encryption_parts();
}

void MainScreen::btn_edit_pwd(){
    passwordDialog->run();
    show_encryption_parts();
}

void MainScreen::btn_del_pwd(){
    encryptPassword.clear();
    show_encryption_parts();
}

void MainScreen::btn_pwd_chosen(){
    if(passwordEntry->get_text_length() == 0){
        errorLabel->show();
        return;
    }
    errorLabel->hide();
    encryptPassword = passwordEntry->get_text();
    passwordEntry->set_text("");
    passwordDialog->hide();
}

void MainScreen::btn_enc_image_chosen(){
    string fileName = encryptionImageChooser->get_filename();
    struct stat info;

    if(stat(fileName.c_str(), &info) != 0){
        return;
    }
    else if(info.st_mode & S_IFDIR){
        encryptionImageChooser->set_current_folder(fileName);
        return;
    }
    else{
        encryptImage = fileName;
        pChooser->hide();
    }
}

void MainScreen::show_encryption_parts(){
    if(!encrypt){
        btnAddImageKey->set_sensitive(false);
        btnEditImageKey->set_sensitive(false);
        btnDelImageKey->set_sensitive(false);
        btnAddPassword->set_sensitive(false);
        btnEditPassword->set_sensitive(false);
        btnDelPassword->set_sensitive(false);
    }
    else{
        if(encryptImage.empty()){
            btnAddImageKey->set_sensitive();
            btnEditImageKey->set_sensitive(false);
            btnDelImageKey->set_sensitive(false);
        }
        else{
            btnAddImageKey->set_sensitive(false);
            btnEditImageKey->set_sensitive();
            btnDelImageKey->set_sensitive();
        }

        if(encryptPassword.empty()){
            btnAddPassword->set_sensitive();
            btnEditPassword->set_sensitive(false);
            btnDelPassword->set_sensitive(false);
        }
        else{
            btnAddPassword->set_sensitive(false);
            btnEditPassword->set_sensitive();
            btnDelPassword->set_sensitive();
        }
    }
}
