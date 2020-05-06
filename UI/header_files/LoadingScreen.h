#ifndef FRANKSTOREUI_LOADINGSCREEN_H
#define FRANKSTOREUI_LOADINGSCREEN_H

#include <gtkmm.h>

class LoadingScreen{
private:
    Gtk::Window* pWindow = nullptr;
    bool valid = false;
public:
    LoadingScreen();
    ~LoadingScreen();
    Gtk::Window* getWindow();
    bool isValid();
    bool readyUp();
};

#endif //FRANKSTOREUI_LOADINGSCREEN_H
