#include <gtk/gtk.h>
#include <gtk-layer-shell/gtk-layer-shell.h>

#include "Taskbar/Taskbar.hpp"
#include "Screen/Screen.hpp"

int main(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    Screen *screen = new Screen();

    Taskbar *taskbar = new Taskbar();
    taskbar->loadStyle("style/Taskbar/index.css");
    taskbar->getScale().x = screen->getWidth() - (screen->getWidth() / 3);
    taskbar->getScale().y = 60;
    taskbar->draw();

    gtk_main();
    return 0;
}
