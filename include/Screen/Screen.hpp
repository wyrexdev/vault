#include <gtk/gtk.h>
#include <iostream>

class Screen {
public:
    Screen();
    
    float getWidth();
    float getHeight();

private:
    float width;
    float height;
};