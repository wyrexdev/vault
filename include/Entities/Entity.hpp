#include <gtk/gtk.h>
#include <string>
#include <glm/glm.hpp>
#include <gtk-layer-shell/gtk-layer-shell.h>

class Entity
{
public:
    Entity();

    void setBackgroundColor(glm::vec4 color);
    void setBackgroundColor(float r, float g, float b, float a);

    glm::vec3& getPosition();
    glm::vec3& getRotation();
    glm::vec2& getScale();

    void loadStyle(const std::string &path);
    void draw();

    virtual void onDraw() = 0;

    GtkWidget *window;

private:
    glm::vec2 scale{1.0f};
    glm::vec3 pos{0.0f}, rot{0.0f};
    glm::vec4 backgroundColor{0.0f, 0.0f, 0.0f, 1.0f};

    std::string stylePath;
};
