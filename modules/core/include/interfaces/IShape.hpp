#ifndef I_SHAPE_HPP
#define I_SHAPE_HPP

#include <string>
#include <memory>

namespace slideEditor::core {

enum class ShapeType {
    CIRCLE,
    RECTANGLE,
    TRIANGLE,
    ELLIPSE
};

inline std::string shapeTypeToString(ShapeType type) {
    switch (type) {
        case ShapeType::CIRCLE: 
            return "circle";
        case ShapeType::RECTANGLE: 
            return "rectangle";
        case ShapeType::TRIANGLE: 
            return "triangle";
        case ShapeType::ELLIPSE: 
            return "ellipse";
        default: 
            return "unknown";
    }
}

inline ShapeType stringToShapeType(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    if (lower == "circle") {
        return ShapeType::CIRCLE;
    }
    if (lower == "rectangle") {
        return ShapeType::RECTANGLE;
    }
    if (lower == "triangle") {
        return ShapeType::TRIANGLE;
    }
    if (lower == "ellipse") {
        return ShapeType::ELLIPSE;
    }
    
    return ShapeType::CIRCLE;  // Default
}

class IShape {
public:
    virtual ~IShape() = default;
    
    virtual ShapeType getType() const = 0;
    virtual double getScale() const = 0;
    virtual void setScale(double scale) = 0;
    virtual std::string getBorderColor() const = 0;
    virtual std::string getFillColor() const = 0;
    virtual void setBorderColor(const std::string& color) = 0;
    virtual void setFillColor(const std::string& color) = 0;
    virtual std::string toString() const = 0;
    virtual std::string serialize() const = 0;
    virtual std::unique_ptr<IShape> clone() const = 0;
    // SVG generation
    virtual std::string toSVG(double x, double y) const = 0;
};

} // namespace slideEditor::core

#endif // I_SHAPE_HPP