#include "Services/Application/ApplicationService.hpp"
#include <iostream>
#include <array>
#include <memory>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <unistd.h>

namespace fs = std::filesystem;

std::vector<WindowInfo> ApplicationService::getOpenWindows()
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
            {
                current.iconPath = getIconFromDesktopEntry(current.appClass);
                if (current.iconPath.empty() && current.pid > 0)
                {
                    current.iconPath = getIconFromPid(current.pid);
                }
                windows.push_back(current);
            }
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
    {
        current.iconPath = getIconFromDesktopEntry(current.appClass);
        if (current.iconPath.empty() && current.pid > 0)
        {
            current.iconPath = getIconFromPid(current.pid);
        }
        windows.push_back(current);
    }

    return windows;
}

std::string ApplicationService::getIconFromDesktopEntry(const std::string &appClass)
{
    std::vector<std::string> dataDirs = {
        std::string(getenv("HOME")) + "/.local/share/applications",
        "/usr/share/applications",
        "/usr/local/share/applications"};

    std::string searchClass = appClass;
    std::transform(searchClass.begin(), searchClass.end(), searchClass.begin(), ::tolower);

    for (const auto &dataDir : dataDirs)
    {
        if (!fs::exists(dataDir)) {
            continue;
        }

        try {
            for (const auto &entry : fs::directory_iterator(dataDir))
            {
                if (entry.path().extension() == ".desktop")
                {
                    std::string filename = entry.path().stem();
                    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

                    if (filename.find(searchClass) != std::string::npos)
                    {
                        std::string icon = parseDesktopFileForIcon(entry.path());
                        if (!icon.empty()) {
                            return icon;
                        }
                    }

                    std::string icon = parseDesktopFileForIcon(entry.path(), searchClass);
                    if (!icon.empty()) {
                        return icon;
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            continue;
        }
    }

    return "";
}

std::string ApplicationService::parseDesktopFileForIcon(const fs::path& desktopPath, const std::string& searchClass)
{
    std::ifstream file(desktopPath);
    std::string line;
    bool inDesktopEntry = false;
    std::string iconName;
    std::string appName;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        if (line == "[Desktop Entry]")
        {
            inDesktopEntry = true;
            iconName.clear();
            appName.clear();
        }
        else if (line[0] == '[' && line != "[Desktop Entry]")
        {
            inDesktopEntry = false;
        }
        else if (inDesktopEntry)
        {
            if (line.find("Icon=") == 0)
            {
                iconName = line.substr(5);
                iconName.erase(iconName.find_last_not_of(" \t\n\r\f\v") + 1);
            }
            else if (line.find("Name=") == 0)
            {
                appName = line.substr(5);
                std::transform(appName.begin(), appName.end(), appName.begin(), ::tolower);
            }
            else if (line.find("Exec=") == 0)
            {
                std::string exec = line.substr(5);
                std::transform(exec.begin(), exec.end(), exec.begin(), ::tolower);
                
                if (exec.find(searchClass) != std::string::npos && !iconName.empty())
                {
                    return findIconPath(iconName);
                }
            }
        }
    }

    if (!searchClass.empty() && !appName.empty() && appName.find(searchClass) != std::string::npos && !iconName.empty())
    {
        return findIconPath(iconName);
    }

    if (searchClass.empty() && !iconName.empty())
    {
        return findIconPath(iconName);
    }

    return "";
}

std::string ApplicationService::findIconPath(const std::string &iconName)
{
    if (fs::exists(iconName)) {
        return iconName;
    }

    std::vector<std::string> iconDirs = {
        std::string(getenv("HOME")) + "/.icons",
        "/usr/share/icons/hicolor",
        "/usr/share/icons/gnome",
        "/usr/share/icons/Adwaita",
        "/usr/share/icons",
        "/usr/share/pixmaps"};

    std::vector<std::string> extensions = {".png", ".svg", ".xpm", ""};
    std::vector<std::string> sizes = {"512x512", "256x256", "128x128", "96x96", "64x64", "48x48", "32x32", "24x24", "16x16", "scalable"};

    for (const auto &iconDir : iconDirs)
    {
        if (!fs::exists(iconDir)) continue;

        for (const auto &size : sizes)
        {
            for (const auto &ext : extensions)
            {
                std::vector<std::string> paths = {
                    iconDir + "/" + size + "/apps/" + iconName + ext,
                    iconDir + "/" + size + "/mimetypes/" + iconName + ext,
                    iconDir + "/" + size + "/categories/" + iconName + ext,
                    iconDir + "/" + size + "/devices/" + iconName + ext
                };

                for (const auto& path : paths) {
                    if (fs::exists(path)) {
                        return path;
                    }
                }
            }
        }

        for (const auto &ext : extensions)
        {
            std::string fullPath = iconDir + "/" + iconName + ext;
            if (fs::exists(fullPath))
            {
                return fullPath;
            }
        }
    }

    for (const auto &ext : extensions)
    {
        std::string fullPath = "/usr/share/pixmaps/" + iconName + ext;
        if (fs::exists(fullPath))
        {
            return fullPath;
        }
    }

    return "";
}

std::string ApplicationService::getIconFromPid(int pid)
{
    std::string exePath = "/proc/" + std::to_string(pid) + "/exe";

    if (fs::exists(exePath))
    {
        char buffer[1024];
        ssize_t len = readlink(exePath.c_str(), buffer, sizeof(buffer) - 1);
        if (len != -1)
        {
            buffer[len] = '\0';
            std::string executable = fs::path(buffer).filename();
            return getIconFromDesktopEntry(executable);
        }
    }
    
    return "";
}