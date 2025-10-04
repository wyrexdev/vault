#include "Entities/Entity.hpp"
#include "Services/Application/ApplicationService.hpp"
#include "Entities/Taskbar/Application.hpp"

#include <gtk-layer-shell/gtk-layer-shell.h>

class Taskbar : public Entity {
public:
    void onDraw() override;
};