#include "view/BrowserOpener.hpp"
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#elif __APPLE__
    #include <unistd.h>
#elif __linux__
    #include <unistd.h>
#endif

namespace slideEditor::view {

bool BrowserOpener::openInBrowser(const std::string& filepath) {
    // Get absolute path
    std::filesystem::path absPath = std::filesystem::absolute(filepath);
    std::string fullPath = absPath.string();
    
#ifdef _WIN32
    // Windows: Use ShellExecute
    std::string command = "start \"\" \"" + fullPath + "\"";
    return executeCommand(command);
    
#elif __APPLE__
    // macOS: Use 'open' command
    std::string command = "open \"" + fullPath + "\"";
    return executeCommand(command);
    
#elif __linux__
    // Linux: Try xdg-open
    std::string command = "xdg-open \"" + fullPath + "\" 2>/dev/null";
    if (!executeCommand(command)) {
        // Fallback to other browsers
        std::vector<std::string> browsers = {
            "firefox", "google-chrome", "chromium-browser", "opera"
        };
        
        for (const auto& browser : browsers) {
            command = browser + " \"" + fullPath + "\" 2>/dev/null &";
            if (executeCommand(command)) {
                return true;
            }
        }

        return false;
    }

    return true;
    
#else
    // Unknown platform
    return false;
#endif
}

std::string BrowserOpener::getBrowserCommand() {
#ifdef _WIN32
    return "start";
#elif __APPLE__
    return "open";
#elif __linux__
    return "xdg-open";
#else
    return "";
#endif
}

bool BrowserOpener::executeCommand(const std::string& command) {
    int result = std::system(command.c_str());
    return result == 0;
}

} // namespace slideEditor::view