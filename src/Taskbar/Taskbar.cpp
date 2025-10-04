#include "Taskbar/Taskbar.hpp"
#include <memory>

void Taskbar::onDraw()
{
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkCssProvider *provider = gtk_css_provider_new();
    std::string css = R"(
        box {
            padding: 10px;
        }
    )";

    gtk_css_provider_load_from_data(provider, css.c_str(), -1, nullptr);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    auto logo = std::make_unique<Image>();
    logo->loadImage("images/logo.png");

    logo->getScale().y = 50;
    logo->getScale().x = 50;

    logo->setParent(main_box);
    logo->onDraw();
}