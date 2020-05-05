#include "header_files/LoadingScreen.h"
#include <iostream>
#include <gtkmm.h>

using namespace std;

int main(int argc, char **argv){
    cout << "Hello World!" << endl;

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    auto css_provider = Gtk::CssProvider::create();

    css_provider->signal_parsing_error().connect(
      [](const Glib::RefPtr<const Gtk::CssSection>& section,
         const Glib::Error& error) {
        std::cerr << "CSS Error: " << error.what() << std::endl;
        std::exit(1);
      });

    css_provider->load_from_path("designs/application.css");

    Gtk::StyleContext::add_provider_for_screen(Gdk::Screen::get_default(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    LoadingScreen loadingScreen;

    if(loadingScreen.isValid()){
        app->run(*(loadingScreen.getWindow()));
    }

    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
