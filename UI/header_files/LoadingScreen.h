#ifndef FRANKSTOREUI_LOADINGSCREEN_H
#define FRANKSTOREUI_LOADINGSCREEN_H

#include <gtkmm.h>

class LoadingScreen{
private:
    Gtk::Window* pWindow = nullptr;
    bool valid = false;

    bool readyUp();
    bool foundFrankstore();
public:
    LoadingScreen();
    ~LoadingScreen();
    Gtk::Window* getWindow();
    bool isValid();
};

#endif //FRANKSTOREUI_LOADINGSCREEN_H
