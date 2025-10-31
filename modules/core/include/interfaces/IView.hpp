#ifndef I_VIEW_HPP
#define I_VIEW_HPP

#include <string>

namespace slideEditor::core {

class ISlideRepository;

class IView {
public:
    virtual ~IView() = default;
    virtual void displayMessage(const std::string& message) = 0;
    virtual void displayError(const std::string& error) = 0;
    virtual void displaySlides(const ISlideRepository* repository) = 0;
    virtual void displayHelp(const std::string& helpText) = 0;
    virtual void displayPrompt() = 0;
};

} // namespace slideEditor::core

#endif // I_VIEW_HPP