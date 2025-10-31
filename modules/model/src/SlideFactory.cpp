#include "model/SlideFactory.hpp"
#include "model/Slide.hpp"
#include "model/shapes/Circle.hpp"
#include "model/shapes/Rectangle.hpp"
#include "model/shapes/Triangle.hpp"
#include "model/shapes/Ellipse.hpp"
#include <algorithm>
#include <cctype>

namespace slideEditor::model {

std::unique_ptr<core::IShape> SlideFactory::createShape(
    const std::string& type, 
    double scale) 
{
    std::string lowerType = type;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    if (lowerType == "circle") {
        return std::make_unique<Circle>(scale);
    } 
    else if (lowerType == "rectangle") {
        return std::make_unique<Rectangle>(scale);
    } 
    else if (lowerType == "triangle") {
        return std::make_unique<Triangle>(scale);
    } 
    else if (lowerType == "ellipse") {
        return std::make_unique<Ellipse>(scale);
    }
    
    return nullptr;
}

std::unique_ptr<core::ISlide> SlideFactory::createSlide(
    int id,
    const std::string& title,
    const std::string& content,
    const std::string& theme)
{
    return std::make_unique<Slide>(id, title, content, theme);
}

bool SlideFactory::isValidShapeType(const std::string& type) {
    std::string lowerType = type;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    return lowerType == "circle" || lowerType == "rectangle" || 
           lowerType == "triangle" || lowerType == "ellipse";
}

} // namespace slieEditor::model