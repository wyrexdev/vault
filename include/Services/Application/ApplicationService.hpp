#pragma once

#include <string>
#include <vector>
#include <filesystem>

struct WindowInfo
{
    std::string title;
    std::string appClass;
    int pid;
    std::string iconPath;
    
    WindowInfo() : pid(0) {}
};

class ApplicationService
{
public:
    static std::vector<WindowInfo> getOpenWindows();
    
private:
    static std::string getIconFromDesktopEntry(const std::string &appClass);
    static std::string parseDesktopFileForIcon(const std::filesystem::path& desktopPath, const std::string& searchClass = "");
    static std::string findIconPath(const std::string &iconName);
    static std::string getIconFromPid(int pid);
};