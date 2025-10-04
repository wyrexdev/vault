#include "Entities/Entity.hpp"
#include <gtk-layer-shell/gtk-layer-shell.h>

bool Entity::gtkInitialized = false;

Entity::Entity() = default;

Entity::~Entity()
{
    if (cssProvider)
    {
        g_object_unref(cssProvider);
    }
}

void Entity::initializeWindow()
{
    if (!gtkInitialized)
    {
        gtk_init(nullptr, nullptr);
        gtkInitialized = true;
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

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

    if (!stylePath.empty())
    {
        cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(cssProvider, stylePath.c_str(), nullptr);
        gtk_style_context_add_provider_for_screen(
            gdk_screen_get_default(),
            GTK_STYLE_PROVIDER(cssProvider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
}

void Entity::draw()
{
    if (parentContainer)
    {
        onDraw();
    }
    else
    {
        initializeWindow();
        onDraw();
        gtk_widget_show_all(window);
        gtk_main();
    }
}

void Entity::onDraw()
{
}

void Entity::setBackgroundColor(glm::vec4 color)
{
    this->backgroundColor = color;
}

void Entity::setBackgroundColor(float r, float g, float b, float a)
{
    this->backgroundColor = glm::vec4(r, g, b, a);
}

void Entity::loadStyle(const std::string &path)
{
    this->stylePath = path;
}