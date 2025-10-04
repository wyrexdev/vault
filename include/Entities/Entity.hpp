#pragma once

#include <glm/glm.hpp>
#include <gtk/gtk.h>
#include <string>

class Entity
{
public:
    Entity();
    virtual ~Entity();

    glm::vec3 &getPosition() { return pos; }
    glm::vec3 &getRotation() { return rot; }
    glm::vec2 &getScale() { return scale; }

    void setBackgroundColor(glm::vec4 color);
    void setBackgroundColor(float r, float g, float b, float a);
    void loadStyle(const std::string &path);

    void setParent(GtkWidget* parent) { parentContainer = parent; }
    
    void initializeWindow();
    void draw();
    virtual void onDraw();

protected:
    glm::vec3 pos{0.0f};
    glm::vec3 rot{0.0f};
    glm::vec2 scale{100.0f, 50.0f};
    glm::vec4 backgroundColor{0.2f, 0.2f, 0.2f, 1.0f};
    std::string stylePath;
    GtkWidget *window{nullptr};
    GtkCssProvider *cssProvider{nullptr};
    GtkWidget *parentContainer{nullptr};

private:
    static bool gtkInitialized;
};