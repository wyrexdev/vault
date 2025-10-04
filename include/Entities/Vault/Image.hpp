#include <gtk/gtk.h>
#include <string>

#include "Entities/Entity.hpp"

class Image : public Entity {
public: 
    void onDraw() override;

    void loadImage(std::string path);

private:
    std::string imagePath;
};