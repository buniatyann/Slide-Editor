#ifndef BROWSER_OPENER_HPP
#define BROWSER_OPENER_HPP

#include <string>

namespace slideEditor::view {

class BrowserOpener {
public:
    static bool openInBrowser(const std::string& filepath);
    static std::string getBrowserCommand(); // Get the command to open browser (platform-specific)

private:
    static bool executeCommand(const std::string& command);
};

} // namespace

#endif // BROWSER_OPENER_HPP