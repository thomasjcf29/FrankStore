#ifndef FRANKSTOREUI_FILECHOOSER_H
#define FRANKSTOREUI_FILECHOOSER_H

#include <gtkmm.h>
#include <string>

class FileChooser{
private:
    Gtk::FileChooserDialog* pChooser = nullptr;
    bool valid = false;
public:
    FileChooser();
    ~FileChooser();
    std::string getFileLocation();
    Gtk::FileChooserDialog* getFileChooser();
    bool isValid();
}

#endif //FRANKSTOREUI_FILECHOOSER_H
