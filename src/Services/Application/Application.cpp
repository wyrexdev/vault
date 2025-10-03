#include "Services/Application/Application.hpp"

std::vector<WindowInfo> getOpenWindows()
{
    std::vector<WindowInfo> windows;

    std::array<char, 256> buffer;
    std::string output;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen("hyprctl clients", "r"), pclose);
    if (!pipe)
    {
        std::cerr << "Failed to run hyprctl\n";
        return windows;
    }

    WindowInfo current;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        std::string line(buffer.data());

        if (line.find("Window") != std::string::npos)
        {
            if (!current.title.empty())
                windows.push_back(current);
            current = WindowInfo();
        }
        else if (line.find("class:") != std::string::npos)
        {
            current.appClass = line.substr(line.find(":") + 2);
            current.appClass.erase(current.appClass.find_last_not_of("\n\r") + 1);
        }
        else if (line.find("title:") != std::string::npos)
        {
            current.title = line.substr(line.find(":") + 2);
            current.title.erase(current.title.find_last_not_of("\n\r") + 1);
        }
        else if (line.find("pid:") != std::string::npos)
        {
            current.pid = std::stoi(line.substr(line.find(":") + 2));
        }
    }

    if (!current.title.empty())
        windows.push_back(current);

    return windows;
}