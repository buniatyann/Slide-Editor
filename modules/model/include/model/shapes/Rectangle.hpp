#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "interfaces/IShape.hpp"
#include "model/BoundingBox.hpp"

namespace slideEditor::model {

class Rectangle : public core::IShape {
public:
    explicit Rectangle(double scale = 1.0);
    
    core::ShapeType getType() const override;
    double getScale() const override;
    std::string toString() const override;
    void setScale(double scale) override;
    std::string serialize() const override;
    std::unique_ptr<core::IShape> clone() const override;
    
    double getWidth() const;
    double getHeight() const;
    BoundingBox getBoundingBox() const;

private:
    double scale_;
    static constexpr double BASE_WIDTH = 2.0;
    static constexpr double BASE_HEIGHT = 1.0;
};

} // namespace slideEditor::model

#endif // RECTANGLE_HPP