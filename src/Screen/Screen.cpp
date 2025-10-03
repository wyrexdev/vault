#include "Screen/Screen.hpp"

Screen::Screen()
{
    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    
    if (!monitor)
    {
        monitor = gdk_display_get_monitor(display, 0);
    }

    if (monitor)
    {
        GdkRectangle geom;
        gdk_monitor_get_geometry(monitor, &geom);

        int width = geom.width;
        int height = geom.height;

        this->width = width;
        this->height = height;
    }
}

float Screen::getWidth()
{
    return width;
}

float Screen::getHeight()
{
    return height;
}