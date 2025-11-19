#ifndef SVG_GENERATOR_HPP
#define SVG_GENERATOR_HPP

#include "interfaces/ISlideRepository.hpp"
#include <string>
#include <memory>

namespace slideEditor::view {

class SvgGenerator {
public:
    SvgGenerator() = default;
    
    std::string generateSVG(const core::ISlideRepository* repository) const; // generate for all slides
    
    // Generate SVG for a single slide
    std::string generateSlideSVG(const core::ISlide* slide, int slideNumber) const;
    
    // Save SVG to file
    bool saveToFile(const std::string& svgContent, const std::string& filename) const;
    
    // Generate and save
    bool generateAndSave(const core::ISlideRepository* repository, 
                        const std::string& filename) const;

private:
    static constexpr int SVG_WIDTH = 800;
    static constexpr int SVG_HEIGHT = 600;
    static constexpr int SHAPE_SPACING_X = 120;
    static constexpr int SHAPE_SPACING_Y = 120;
    static constexpr int START_X = 100;
    static constexpr int START_Y = 150;
};

} // namespace slideEditor::view

#endif  // SVG_GENERATOR