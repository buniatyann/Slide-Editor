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

class IShape {
public:
    virtual ~IShape() = default;
    
    virtual ShapeType getType() const = 0;
    virtual double getScale() const = 0;
    virtual std::string toString() const = 0;
    
    virtual void setScale(double scale) = 0;
    
    virtual std::string serialize() const = 0;
    
    virtual std::unique_ptr<IShape> clone() const = 0;
};

} // namespace slideEditor::core

#endif // I_SHAPE_HPP