#ifndef SLIDE_FACTORY_HPP
#define SLIDE_FACTORY_HPP

#include "interfaces/IShape.hpp"
#include "interfaces/ISlide.hpp"
#include <memory>
#include <string>

namespace slideEditor::model {

class SlideFactory {
public:
    static std::unique_ptr<core::IShape> createShape(
        const std::string& type, 
        double scale,
        const std::string& borderColor = "black",
        const std::string& fillColor = "white"
    );
    
    static std::unique_ptr<core::ISlide> createSlide(
        int id,
        const std::string& title,
        const std::string& content,
        const std::string& theme
    );
    
    static bool isValidShapeType(const std::string& type);
};

} // namespace slideEditor::model

#endif // SLIDE_FACTORY_HPP