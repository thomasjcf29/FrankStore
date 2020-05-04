#include <iostream>
#include <gtkmm.h>

using namespace std;

int main(int argc, char **argv){
    cout << "Hello World!" << endl;

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

    //Load the GtkBuilder file and instantiate its widgets:
    auto refBuilder = Gtk::Builder::create();

    try{
        refBuilder->add_from_file("designs/loading_screen.glade");
    }
    catch(const Glib::FileError& ex){
        std::cerr << "FileError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Glib::MarkupError& ex)
    {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        return 1;
    }
    catch(const Gtk::BuilderError& ex)
    {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        return 1;
    }

    Gtk::Window* pWindow = nullptr;
    refBuilder->get_widget("loadingScreen", pWindow);
    if(pWindow){
        cout << "Found Window" << endl;
        pWindow.show_all();
        app->run(*pWindow);
    }

    delete pWindow;
    return 0;
}

// called when window is closed
void on_window_main_destroy()
{
    gtk_main_quit();
}
