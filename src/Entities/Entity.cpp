#include "Entities/Entity.hpp"

Entity::Entity() = default;

void Entity::setBackgroundColor(glm::vec4 color)
{
    this->backgroundColor = color;
}

void Entity::setBackgroundColor(float r, float g, float b, float a)
{
    this->backgroundColor = glm::vec4(r, g, b, a);
}

glm::vec3 &Entity::getPosition() { return pos; }
glm::vec3 &Entity::getRotation() { return rot; }
glm::vec2 &Entity::getScale() { return scale; }

void Entity::loadStyle(const std::string &path)
{
    this->stylePath = path;
}

void Entity::draw()
{
    gtk_init(nullptr, nullptr);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    onDraw();

    int width = static_cast<int>(scale.x);
    int height = static_cast<int>(scale.y);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    gtk_widget_set_size_request(window, width, height);

    gtk_window_move(GTK_WINDOW(window), static_cast<int>(pos.x), static_cast<int>(pos.y));

    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    gtk_layer_init_for_window(GTK_WINDOW(window));
    gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_LAYER_BOTTOM);
    gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_BOTTOM, TRUE);

    gtk_layer_set_exclusive_zone(GTK_WINDOW(window), height);

    GtkCssProvider *provider = gtk_css_provider_new();
    if (!stylePath.empty())
    {
        gtk_css_provider_load_from_path(provider, stylePath.c_str(), nullptr);
        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
    gtk_widget_show_all(window);
    gtk_main();
}
