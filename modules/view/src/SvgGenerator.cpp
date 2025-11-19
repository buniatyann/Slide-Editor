#include "view/SvgGenerator.hpp"
#include <fstream>
#include <sstream>
#include <cmath>

namespace slideEditor::view {

std::string SvgGenerator::generateSVG(const core::ISlideRepository* repository) const {
    if (!repository) return "";
    
    const auto& slides = repository->getAllSlides();
    if (slides.empty()) {
        return R"(<?xml version="1.0" encoding="UTF-8"?>
        <svg xmlns="http://www.w3.org/2000/svg" width="800" height="600">
        <rect width="100%" height="100%" fill="#f0f0f0"/>
        <text x="400" y="300" text-anchor="middle" font-size="24" fill="#333">
        No slides to display
        </text>
        </svg>)";
    }
    
    std::ostringstream svg;
    
    // SVG header
    int totalHeight = slides.size() * (SVG_HEIGHT + 50);
    svg << R"(<?xml version="1.0" encoding="UTF-8"?>)" << "\n";
    svg << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << SVG_WIDTH 
        << R"(" height=")" << totalHeight << R"(">)" << "\n";
    
    // Background
    svg << R"(  <rect width="100%" height="100%" fill="#f5f5f5"/>)" << "\n\n";
    int slideNumber = 0;
    for (const auto& slide : slides) {
        svg << generateSlideSVG(slide.get(), slideNumber);
        slideNumber++;
    }
    
    // SVG footer
    svg << "</svg>\n";
    return svg.str();
}

std::string SvgGenerator::generateSlideSVG(const core::ISlide* slide, int slideNumber) const {
    std::ostringstream svg;
    int offsetY = slideNumber * (SVG_HEIGHT + 50);
    // Slide group
    svg << "  <g id=\"slide-" << slide->getId() << "\">\n";

    // Slide background
    svg << "    <rect x=\"10\" y=\"" << (offsetY + 10) 
        << "\" width=\"" << (SVG_WIDTH - 20) 
        << "\" height=\"" << (SVG_HEIGHT - 20) 
        << R"(" fill="white" stroke="#333" stroke-width="2" rx="5"/>)" << "\n";
    
    // Slide title
    svg << "    <text x=\"" << (SVG_WIDTH / 2) << "\" y=\"" << (offsetY + 40) 
        << R"(" text-anchor="middle" font-size="28" font-weight="bold" fill="#333">)"
        << slide->getTitle() << "</text>\n";
    
    // Slide content
    svg << "    <text x=\"" << (SVG_WIDTH / 2) << "\" y=\"" << (offsetY + 70) 
        << R"(" text-anchor="middle" font-size="18" fill="#666">)"
        << slide->getContent() << "</text>\n";
    
    // Theme indicator
    svg << "    <text x=\"20\" y=\"" << (offsetY + SVG_HEIGHT - 20) 
        << R"(" font-size="14" fill="#999">Theme: )" << slide->getTheme() << "</text>\n";
    
    // Shapes
    const auto& shapes = slide->getShapes();
    if (!shapes.empty()) {
        int shapesPerRow = std::min(5, static_cast<int>(shapes.size()));
        int rows = (shapes.size() + shapesPerRow - 1) / shapesPerRow;
        for (size_t i = 0; i < shapes.size(); ++i) {
            int row = i / shapesPerRow;
            int col = i % shapesPerRow;
            double x = START_X + col * SHAPE_SPACING_X;
            double y = offsetY + START_Y + row * SHAPE_SPACING_Y;
            
            svg << "    " << shapes[i]->toSVG(x, y) << "\n";
        }
    }
    
    svg << "  </g>\n\n";
    return svg.str();
}

bool SvgGenerator::saveToFile(const std::string& svgContent, const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << svgContent;
    file.close();
    
    return true;
}

bool SvgGenerator::generateAndSave(const core::ISlideRepository* repository, 
                                  const std::string& filename) const {
    std::string svgContent = generateSVG(repository);
    return saveToFile(svgContent, filename);
}

} // namespace slideEditor::view