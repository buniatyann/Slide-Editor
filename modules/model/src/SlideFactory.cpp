#include "model/SlideFactory.hpp"
#include "model/Slide.hpp"
#include "model/shapes/Shape.hpp"
#include <algorithm>
#include <cctype>

namespace slideEditor::model {

std::unique_ptr<core::IShape> SlideFactory::createShape(
    const std::string& type, 
    double scale,
    const std::string& borderColor,
    const std::string& fillColor) 
{
    std::string lowerType = type;
    std::transform(lowerType.begin(), lowerType.end(), lowerType.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    core::ShapeType shapeType;
    if (lowerType == "circle") {
        shapeType = core::ShapeType::CIRCLE;
    } 
    else if (lowerType == "rectangle") {
        shapeType = core::ShapeType::RECTANGLE;
    } 
    else if (lowerType == "triangle") {
        shapeType = core::ShapeType::TRIANGLE;
    } 
    else if (lowerType == "ellipse") {
        shapeType = core::ShapeType::ELLIPSE;
    } 
    else {
        return nullptr;
    }
    
    Color border = Color::fromString(borderColor);
    Color fill = Color::fromString(fillColor);
    
    return std::make_unique<Shape>(shapeType, scale, border, fill);
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