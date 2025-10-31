#ifndef CLI_VIEW_HPP
#define CLI_VIEW_HPP

#include "interfaces/IView.hpp"
#include "interfaces/ISlideRepository.hpp"
#include <iostream>

namespace slideEditor::view {

class CliView : public core::IView {
public:
    explicit CliView(std::ostream& output = std::cout);
    
    void displayMessage(const std::string& message) override;
    void displayError(const std::string& error) override;
    void displaySlides(const core::ISlideRepository* repository) override;
    void displayHelp(const std::string& helpText) override;
    void displayPrompt() override;

private:
    std::ostream& output_;
    
    // Formatting helpers
    std::string formatSlide(const core::ISlide* slide) const;
    std::string formatShape(const core::IShape* shape, size_t index) const;
};

} // namespace

#endif // CLI_VIEW_HPP