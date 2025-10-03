#include <string>
#include <vector>
#include <memory>
#include <array>
#include <iostream>

struct WindowInfo
{
    std::string title;
    std::string appClass;
    int pid;
};

class Application
{
public:
    std::vector<WindowInfo> getOpenWindows();
};