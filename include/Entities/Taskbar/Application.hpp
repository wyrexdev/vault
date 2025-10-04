#include "Entities/Entity.hpp"
#include "Entities/Vault/Image.hpp"

#include <memory>

class Application : public Entity
{
public:
    Application(GtkWidget* parent);

    void onDraw() override;
    void setImage(std::string path);

private:
    GtkWidget *parent;
    std::string imgPath;
};