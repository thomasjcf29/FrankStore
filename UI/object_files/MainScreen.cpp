#include "../header_files/MainScreen.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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
    refBuilder->get_widget("confirmationDialog", confirmationDialog);
    refBuilder->get_widget("filesToHideChooser", filesToHideChooser);
    refBuilder->get_widget("boxOfFiles", boxOfFiles);

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

    //Cover Image Selection
    refBuilder->get_widget("btnCoverImage", btnCoverImage);

    //Encryption Image Section
    refBuilder->get_widget("btnOpenEncryptionImage", btnOpenEncryptionImage);

    //File Addition
    refBuilder->get_widget("btnAddFiles", btnAddFiles);
    refBuilder->get_widget("btnFilesChosen", btnFilesChosen);
    refBuilder->get_widget("btnFolderNo", btnFolderNo);
    refBuilder->get_widget("btnFolderYes", btnFolderYes);

    //File Removal
    refBuilder->get_widget("btnDelFiles", btnDelFiles);

    if(!btnAddImageKey || !btnEditImageKey || !btnDelImageKey || !btnAddPassword ||
       !btnEditPassword || !btnDelPassword || !chkEncryption || !pneController ||
       !passwordDialog || !btnPasswordChosen || !passwordEntry || !errorLabel ||
       !encryptionImageChooser || !btnOpenEncryptionImage || !btnCoverImage || !confirmationDialog ||
       !filesToHideChooser || !btnFilesChosen || !btnFolderNo || !btnFolderYes ||
       !btnDelFiles || !boxOfFiles){
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

    btnCoverImage->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_sel_cover_image));

    btnAddFiles->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::show_file_chooser));
    btnFilesChosen->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_file_choosen));
    btnFolderNo->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_folder_no));
    btnFolderYes->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_folder_yes));

    btnDelFiles->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_del_files));

    Gtk::IconSize::register_new("file_icon_layout", 20, 20);

    if(pWindow){
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        pWindow->set_size_request(900, 540);
        pneController->set_position(550);
        valid = true;
    }
}

MainScreen::~MainScreen(){
    delete pWindow;
    delete gridEncryption;
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
    delete btnCoverImage;
    delete confirmationDialog;
    delete filesToHideChooser;
    delete btnFilesChosen;
    delete btnFolderNo;
    delete btnFolderYes;
    delete btnAddFiles;
    delete btnDelFiles;
    delete boxOfFiles;
}

Gtk::Window* MainScreen::getWindow(){
    return pWindow;
}

bool MainScreen::isValid(){
    return valid;
}

void MainScreen::add_files(string path, string folder){
    DIR *dir;
    struct dirent *ent;
    if((dir = opendir(folder.c_str())) != NULL){
        while((ent = readdir (dir)) != NULL) {
            if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0){
                continue;
            }

            string result = path + separator + ent->d_name;

            if(isDir(result)){
                add_files(result, result);
            }
            else{
                files.push_back(result);
                shortFileName.push_back(ent->d_name);
            }
        }
        closedir(dir);
    }
}

bool MainScreen::isDir(string location){
    struct stat info;

    if(stat(location.c_str(), &info) != 0){
        return false;
    }
    else if(info.st_mode & S_IFDIR){
        return true;
    }
    else{
        return false;
    }
}

void MainScreen::checkbox_encryption_toggled(){
    encrypt = !encrypt;
    show_encryption_parts();
}

void MainScreen::btn_add_image(){
    choosingEncryptImage = true;
    encryptionImageChooser->run();
    show_encryption_parts();
}

void MainScreen::btn_edit_image(){
    choosingEncryptImage = true;
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

void MainScreen::btn_sel_cover_image(){
    choosingCoverImage = true;
    encryptionImageChooser->run();
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
        if(choosingCoverImage){
            coverImage = fileName;
        }
        if(choosingEncryptImage){
            encryptImage = fileName;
        }
        choosingCoverImage = false;
        choosingEncryptImage = false;
        encryptionImageChooser->hide();
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

void MainScreen::show_file_chooser(){
    confirmFolder = false;
    filesToHideChooser->run();
    add_files_to_screen();
}

void MainScreen::btn_file_choosen(){
    string fileName = filesToHideChooser->get_filename();
    struct stat info;

    if(stat(fileName.c_str(), &info) != 0){
        return;
    }
    else if(info.st_mode & S_IFDIR){
        //Folder Show Dialog
        confirmationDialog->run();
        if(confirmFolder){
            add_files(fileName, fileName);
            filesToHideChooser->hide();
        }
        else{
            return;
        }
    }
    else{
        size_t lastPosition = fileName.find_last_of(separator);
        string sFileName = fileName.substr(lastPosition+1);
        files.push_back(fileName);
        shortFileName.push_back(sFileName);
        filesToHideChooser->hide();
    }
}

void MainScreen::btn_folder_no(){
    confirmFolder = false;
    confirmationDialog->hide();
}

void MainScreen::btn_folder_yes(){
    confirmFolder = true;
    confirmationDialog->hide();
}

void MainScreen::btn_del_files(){
    files.clear();
    shortFileName.clear();
    add_files_to_screen();
}

void MainScreen::add_files_to_screen(){

    vector<Gtk::Widget*> children = boxOfFiles->get_children();
    size_t oldFiles = children.size();
    size_t size = shortFileName.size();

    for(size_t i = 0; i < oldFiles; i++){
        delete children[i];
    }

    if(size == 0){
        Gtk::Label* label = new Gtk::Label("Currently there are no files selected.\nPlease complete step 4.");
        label->set_justify(Gtk::Justification::JUSTIFY_CENTER);
        label->show();
        boxOfFiles->pack_start(*label, false, true);
    }

    else{
        for(size_t i = 0; i < size; i++){
            Gtk::Grid* grid = new Gtk::Grid();
            grid->set_halign(Gtk::Align::ALIGN_FILL);
            grid->set_column_spacing(5);
            grid->set_border_width(5);

            //Image Status (Pending)
            Gtk::Image* image = new Gtk::Image();
            image->set_from_icon_name("media-playback-pause", Gtk::IconSize::from_name("file_icon_layout"));
            image->show();

            //Label For File Status
            Gtk::Label* statusLabel = new Gtk::Label("Pending");
            statusLabel->show();

            //Label For File Name
            Gtk::Label* fileLabel = new Gtk::Label(shortFileName[i]);
            fileLabel->set_alignment(0.95, 0.5);
            fileLabel->set_justify(Gtk::Justification::JUSTIFY_CENTER);
            fileLabel->set_hexpand(true);
            fileLabel->set_line_wrap_mode(Pango::WRAP_WORD_CHAR);
            fileLabel->set_line_wrap(true);
            fileLabel->show();

            grid->attach(*image, 0, 0, 1, 1);
            grid->attach(*statusLabel, 1, 0, 1, 1);
            grid->attach(*fileLabel, 2, 0, 1, 1);
            grid->show();

            boxOfFiles->pack_start(*grid, false, false);
        }
    }
}
