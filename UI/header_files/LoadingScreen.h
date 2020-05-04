#ifndef FRANKSTOREUI_LOADINGSCREEN_H
#define FRANKSTOREUI_LOADINGSCREEN_H

#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/builder.h>

class LoadingScreen : public Gtk::GtkWindow {
private:
    Glib::RefPtr<Gtk::Builder> ui;
    void on_loadingScreen_destroy();
public:
    LoadingScreen() : ui{Gtk::Builder::create_from_file("designs\\loading_screen.glade")};
    ~LoadingScreen();
};


#endif //FRANKSTOREUI_LOADINGSCREEN_H
