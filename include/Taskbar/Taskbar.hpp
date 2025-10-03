#include "Entities/Entity.hpp"

#include <gtk-layer-shell/gtk-layer-shell.h>
class Taskbar : public Entity {
public:
    void onDraw() override;
};