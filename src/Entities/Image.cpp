#include "Entities/Image.hpp"
#include <gtk/gtk.h>

void Image::onDraw()
{
    GtkWidget* target_container = parentContainer ? parentContainer : window;
    GtkWidget* pic;
    
    int target_width = static_cast<int>(getScale().x);
    int target_height = static_cast<int>(getScale().y);
    
    GdkPixbuf *original_pixbuf = gdk_pixbuf_new_from_file(imagePath.c_str(), nullptr);
    if (original_pixbuf) {
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(
            original_pixbuf, target_width, target_height, GDK_INTERP_BILINEAR
        );
        pic = gtk_image_new_from_pixbuf(scaled_pixbuf);
        g_object_unref(original_pixbuf);
        g_object_unref(scaled_pixbuf);
    } else {
        pic = gtk_image_new_from_file(imagePath.c_str());
        gtk_widget_set_size_request(pic, target_width, target_height);
    }
    
    gtk_container_add(GTK_CONTAINER(target_container), pic);
    
    if (!parentContainer) {
        gtk_widget_show_all(window);
    }
}

void Image::loadImage(std::string path) {
    this->imagePath = path;
}