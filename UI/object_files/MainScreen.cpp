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

#include "../header_files/MainScreen.h"
#include "../header_files/FrankThreader.h"
#include "../header_files/StaticFunctions.h"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sstream>

using namespace std;

/**
Constructor for the main screen, openings the resource files and gathers all
relevant objects from the GUI. Do not use if isValid() is false. Also sets up
button listeners etc.
*/
MainScreen::MainScreen(string application){
    executableLocation = application;
    threadManager = new FrankThreader(this);

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
    refBuilder->get_widget("actionInProgress", actionInProgress);
    refBuilder->get_widget("scrollingWindow", scrollingWindow);

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

    //Output Image
    refBuilder->get_widget("chkOutputImage", chkOutputImage);

    //Control Buttons
    refBuilder->get_widget("btnEncode", btnEncode);
    refBuilder->get_widget("btnDecode", btnDecode);

    if(!btnAddImageKey || !btnEditImageKey || !btnDelImageKey || !btnAddPassword ||
       !btnEditPassword || !btnDelPassword || !chkEncryption || !pneController ||
       !passwordDialog || !btnPasswordChosen || !passwordEntry || !errorLabel ||
       !encryptionImageChooser || !btnOpenEncryptionImage || !btnCoverImage || !confirmationDialog ||
       !filesToHideChooser || !btnFilesChosen || !btnFolderNo || !btnFolderYes ||
       !btnDelFiles || !boxOfFiles || !chkOutputImage || !btnEncode ||
       !btnDecode || !actionInProgress || !scrollingWindow){
        cout << "[ERROR]: Invalid glade file!" << endl;
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

    chkOutputImage->signal_toggled().connect(sigc::mem_fun(*this, &MainScreen::checkbox_image_toggled));

    btnEncode->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_encode_pressed));
    btnDecode->signal_clicked().connect(sigc::mem_fun(*this, &MainScreen::btn_decode_pressed));

    dispatcher.connect(sigc::mem_fun(*this, &MainScreen::displayUIProgress));
    dispatcherUIClose.connect(sigc::mem_fun(*this, &MainScreen::hideUIPopup));

    if(pWindow){
        Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        pWindow->set_size_request(1000, 510);
        pneController->set_position(600);
        valid = true;
    }
}

/**
Destructor for main screen, used to delete any resources created by itself.
*/
MainScreen::~MainScreen(){
    delete pWindow;
}

/**
@return the pWindow object which can be displayed by the Application.
*/
Gtk::Window* MainScreen::getWindow(){
    return pWindow;
}

/**
@return if the object is ok to use (true if ok, false if not).
*/
bool MainScreen::isValid(){
    return valid;
}

/**
Loop through files and add them to the vector of files to be encoded / decoded.
This function calls itself if the child item is a subdirectory.
@param string path: Path of the current folder to check.
@param string folder: Path of the parent folder.
*/
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

/**
Return true if the if the item passed in is a folder.
@param string location: Location of file / folder to check.
@return true if folder, false if not.
*/
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

/**
Function called when the check for encryption is toggled.
*/
void MainScreen::checkbox_encryption_toggled(){
    encrypt = !encrypt;
    show_encryption_parts();
    set_form_ready();
}

/**
Function called when the button to add an image as the encryption key is pressed.
*/
void MainScreen::btn_add_image(){
    choosingEncryptImage = true;
    encryptionImageChooser->run();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to edit the previous encryption key is pressed.
*/
void MainScreen::btn_edit_image(){
    choosingEncryptImage = true;
    encryptionImageChooser->run();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to delete the encryption key is pressed.
*/
void MainScreen::btn_del_image(){
    encryptImage.clear();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to add an encrypted password is pressed.
*/
void MainScreen::btn_add_pwd(){
    passwordDialog->run();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to edit an encrypted password is pressed.
*/
void MainScreen::btn_edit_pwd(){
    passwordDialog->run();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to delete an encrypted password is pressed.
*/
void MainScreen::btn_del_pwd(){
    encryptPassword.clear();
    show_encryption_parts();
    set_form_ready();
}

/**
Function called for when the button to submit chosen password is pressed.
(On the password entry dialog.)
*/
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

/**
Function called for when the button to submit chosen image is pressed.
(On the image chooser dialog.)
*/
void MainScreen::btn_sel_cover_image(){
    choosingCoverImage = true;
    encryptionImageChooser->run();
    set_form_ready();
}

/**
Function called for when the button to submit chosen image is pressed.
(On the image chooser dialog.)
*/
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

/**
Function called interally to hide / show the relevant parts of the encryption section.
(Section 2).
*/
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

/**
Function called when the button to add files to the program has been pressed.
*/
void MainScreen::show_file_chooser(){
    confirmFolder = false;
    filesToHideChooser->run();
    add_files_to_screen();
    set_form_ready();
}

/**
Function called when the select file / folder is pressed on the choose your files
dialog.
*/
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
            {
                unique_lock<mutex> lock(fileLock);
                add_files(fileName, fileName);
            }
            filesToHideChooser->hide();
        }
        else{
            return;
        }
    }
    else{
        size_t lastPosition = fileName.find_last_of(separator);
        string sFileName = fileName.substr(lastPosition+1);
        {
            unique_lock<mutex> lock(fileLock);
            files.push_back(fileName);
            shortFileName.push_back(sFileName);
        }
        filesToHideChooser->hide();
    }
}

/**
Function called if user chooses not to add a folder (on the folder choose dialog).
*/
void MainScreen::btn_folder_no(){
    confirmFolder = false;
    confirmationDialog->hide();
}

/**
Function called if user chooses to hide folder (on the folder chooser dialog).
*/
void MainScreen::btn_folder_yes(){
    confirmFolder = true;
    confirmationDialog->hide();
}

/**
Function to clear the current list of files to add.
Called by the clear files button on the UI.
*/
void MainScreen::btn_del_files(){
    {
        unique_lock<mutex> lock(fileLock);
        files.clear();
        shortFileName.clear();
    }
    add_files_to_screen();
    set_form_ready();
}

/**
Displays the current list of files in the vector on the screen.
If there are no files in the screen, then it tells the user to select some.
*/
void MainScreen::add_files_to_screen(){
    {
        unique_lock<mutex> lock(fileLock);

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
                image->set_from_resource("/images/designs/pending.png");
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
}

/**
Function called when the checkbox to choose to hide via image is toggled.
*/
void MainScreen::checkbox_image_toggled(){
    outputImage = !outputImage;
    set_form_ready();
}

/**
Function called each time a UI button is pressed. If the form is ready to go
(all required options) are inputted, the form is enabled for the user and they
can click encode or decode.
*/
void MainScreen::set_form_ready(){

    //Check Sections 1 & 2
    if(encrypt){
        if(encryptPassword.size() == 0 && encryptImage.size() == 0){
            disable_form();
            return;
        }
    }

    //Check Sections 3
    if(coverImage.size() == 0){
        disable_form();
        return;
    }

    //Check Sections 4
    {
        unique_lock<mutex> lock(fileLock);
        if(files.size() == 0 || shortFileName.size() == 0){
            disable_form();
            return;
        }
    }

    disable_form(false);
}

/**
Function to disable the form currently on the screen.
Default if no param is to disable it.
@param bool disable: disable form (true) or enable form (false).
*/
void MainScreen::disable_form(bool disable){
    btnDecode->set_sensitive(!disable);
    btnEncode->set_sensitive(!disable);
}

/**
Function called when the encode button is pressed.
*/
void MainScreen::btn_encode_pressed(){
    highChildPos = 0; //Reset For Next Run
    highHeight = 0; //Reset For Next Run

    if(encrypt){
        actionToDo = FileEncryptAndEncode;
    }
    else{
        actionToDo = FileEncode;
    }

    actionInProgress->show();
    thread fileJob = thread(&MainScreen::calcuate_jobs, this);
    fileJob.detach();
}

/**
Function called when the decode button is pressed.
*/
void MainScreen::btn_decode_pressed(){
    highChildPos = 0; //Reset For Next Run
    highHeight = 0; //Reset For Next Run

    if(encrypt){
        actionToDo = FileDecodeAndDecrypt;
    }
    else{
        actionToDo = FileDecode;
    }

    actionInProgress->show();
    thread fileJob = thread(&MainScreen::calcuate_jobs, this);
    fileJob.detach();
}

/**
Calculates all jobs which need to be created, based on the files vector and then
packs the jobs into a JobStructure ready to be handed to the Thread Manager.
This function should be called asynchronously as it may take a while.
*/
void MainScreen::calcuate_jobs(){
    {
        unique_lock<mutex> lock(fileLock);
        jobsAdded = false;
        for(int i = 0; i < files.size(); i++){
            JobStruct job;
            job.fileName = executableLocation;
            job.sourceFile = files[i];
            job.imageCover = coverImage;
            job.imageEncrypt = encryptImage;
            job.passwordEncrypt = encryptPassword;
            job.outputImage = outputImage;
            job.childNumber = i;
            job.action = actionToDo;
            job.stageAt = NotStarted;
            threadManager->addJob(job);
        }
        jobsAdded = true;
    }
}

/**
Inform the UI to update the progress of this child row with the relevant update message.
This function does not directly update the UI, rather it calls a dispatcher and
asks it do be done on the GUI thread.
@param int cN: The child number of the Gtk::Box to update.
@param UpdateMessage uM: The message you have for that box.
*/
void MainScreen::updateUIProgress(int cN, UpdateMessage uM){
    {
        unique_lock<mutex> lock(fileLock);
        childToUpdate.push_back(cN);
        childToUpdateMessage.push_back(uM);
        dispatcher.emit();
    }
}

/**
Update UI progress from the vector list containing all updates waiting to be displayed.
Function automatically scrolls down to the last row to have been used, although tries
to keep the last file in the middle of the list where possible.
*/
void MainScreen::displayUIProgress(){
    {
        {
            unique_lock<mutex> lock(fileLock);

            for(int i = 0; i < childToUpdate.size(); i++){
                int number = childToUpdate[i];
                if(number > highChildPos){
                    highChildPos = number;
                }

                auto grid = (Gtk::Grid*) boxOfFiles->get_children()[number];

                int height = boxOfFiles->get_children()[number]->get_allocated_height();

                if(height > highHeight){
                    highHeight = height;
                }

                Gtk::Image* icon = (Gtk::Image*) grid->get_child_at(0, 0);
                Gtk::Label* label = (Gtk::Label*) grid->get_child_at(1, 0);

                UpdateMessage updateMessage = childToUpdateMessage[i];

                if(updateMessage == Error){
                    icon->set_from_resource("/images/designs/error.png");
                    label->set_label("Failed");
                }
                else if(updateMessage == InProgress){
                    icon->set_from_resource("/images/designs/inprogress.png");
                    label->set_label("In Progress");
                }
                else if(updateMessage == Success){
                    icon->set_from_resource("/images/designs/success.png");
                    label->set_label("Completed");
                }
            }
            childToUpdate.clear();
            childToUpdateMessage.clear();
        }

        auto adj = scrollingWindow->get_vadjustment();
        double scrollSize = (highHeight + 10) * (highChildPos - 6); //10 for spacing, 6 to allow previous files shown
        adj->set_value(scrollSize);
    }
}

/**
Hide the in progress blocker, preventing the user from doing any work.
*/
void MainScreen::hideUIPopup(){
    actionInProgress->hide();
}

/**
Public call from an ASync thread to say you can hide the UI in progress blocker.
It dispatches an event to the GUI thread to ask it to close if all files have finished adding.
jobsAdded set by calculate_jobs();
*/
void MainScreen::closeUIPopup(){
    unique_lock<mutex> lock(fileLock);
    if(jobsAdded){
        dispatcherUIClose.emit();
    }
}
