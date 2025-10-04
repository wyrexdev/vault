#include "Entities/Taskbar/Application.hpp"

Application::Application(GtkWidget* parent) {
    this->parent = parent;
}

void Application::onDraw()
{
    auto logo = std::make_unique<Image>();
    logo->loadImage(imgPath);

    logo->getScale().y = 35;
    logo->getScale().x = 35;

    logo->setParent(parent);

    logo->onDraw();
}

void Application::setImage(std::string path) {
    this->imgPath = path;
}
