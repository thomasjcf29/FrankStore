#ifndef FRANKSTOREUI_MAINSCREEN_H
#define FRANKSTOREUI_MAINSCREEN_H

#include <gtkmm.h>
#include <string>

class MainScreen{
private:
    std::string executableLocation, coverImage, encryptPassword, encryptImage;
    bool valid = false;
    bool encrypt = false;
    Gtk::Window* pWindow = nullptr;
public:
    MainScreen(std::string application);
    ~MainScreen();
    Gtk::Window* getWindow();
    bool isValid();
};

#endif //FRANKSTOREUI_MAINSCREEN_H
