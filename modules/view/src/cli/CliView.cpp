#include "view/cli/CliView.hpp"
#include <iomanip>
#include <sstream>

namespace slideEditor::view {

CliView::CliView(std::ostream& output) : output_(output) {}

void CliView::displayMessage(const std::string& message) {
    output_ << message << std::endl;
}

void CliView::displayError(const std::string& error) {
    output_ << "[ERROR] " << error << std::endl;
}

void CliView::displaySlides(const core::ISlideRepository* repository) {
    if (!repository) {
        displayError("Repository not available");
        return;
    }
    
    const auto& slides = repository->getAllSlides();
    if (slides.empty()) {
        output_ << "No slides in presentation." << std::endl;
        return;
    }
    
    output_ << "\n========================================\n";
    output_ << "  PRESENTATION (" << slides.size() << " slide(s))\n";
    output_ << "========================================\n\n";
    for (const auto& slide : slides) {
        output_ << formatSlide(slide.get()) << "\n";
    }
}

void CliView::displayHelp(const std::string& helpText) {
    output_ << "\n" << helpText << std::endl;
}

void CliView::displayPrompt() {
    output_ << "> " << std::flush;
}

std::string CliView::formatSlide(const core::ISlide* slide) const {
    std::ostringstream oss;
    oss << "Slide #" << slide->getId() << "\n";
    oss << "  Title: " << slide->getTitle() << "\n";
    oss << "  Content: " << slide->getContent() << "\n";
    oss << "  Theme: " << slide->getTheme() << "\n";
    oss << "  Shapes (" << slide->getShapeCount() << "):\n";
    
    const auto& shapes = slide->getShapes();
    if (shapes.empty()) {
        oss << "    (none)\n";
    } 
    else {
        for (size_t i = 0; i < shapes.size(); ++i) {
            oss << "    " << formatShape(shapes[i].get(), i) << "\n";
        }
    }
    
    return oss.str();
}

std::string CliView::formatShape(const core::IShape* shape, size_t index) const {
    std::ostringstream oss;
    oss << "[" << index << "] " << shape->toString();
    
    return oss.str();
}

} // namespace slideEditor::view