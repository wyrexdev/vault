#include "Taskbar/Taskbar.hpp"

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

    for(WindowInfo application : ApplicationService::getOpenWindows()) {
        Application *app = new Application(main_box);
        app->setImage(application.iconPath);
        app->onDraw();
    }
}