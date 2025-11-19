#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "interfaces/IShape.hpp"
#include "model/Color.hpp"
#include <string>
#include <memory>

namespace slideEditor::model {

class Shape : public core::IShape {
public:
    Shape(core::ShapeType type, 
          double scale = 1.0,
          const Color& borderColor = Color::Black(),
          const Color& fillColor = Color::White());
    
    core::ShapeType getType() const override;
    double getScale() const override;
    void setScale(double scale) override;
    
    std::string getBorderColor() const override;
    std::string getFillColor() const override;
    void setBorderColor(const std::string& color) override;
    void setFillColor(const std::string& color) override;
    
    std::string toString() const override;
    std::string serialize() const override;
    std::unique_ptr<core::IShape> clone() const override;
    
    std::string toSVG(double x, double y) const override;
    
    double getCircleRadius() const;
    double getRectangleWidth() const;
    double getRectangleHeight() const;
    double getTriangleSide() const;
    double getEllipseRadiusX() const;
    double getEllipseRadiusY() const;

protected:
    core::ShapeType type_;
    double scale_;
    Color borderColor_;
    Color fillColor_;
    
    static constexpr double CIRCLE_BASE_RADIUS = 50.0;
    static constexpr double RECTANGLE_BASE_WIDTH = 100.0;
    static constexpr double RECTANGLE_BASE_HEIGHT = 60.0;
    static constexpr double TRIANGLE_BASE_SIDE = 80.0;
    static constexpr double ELLIPSE_BASE_RADIUS_X = 75.0;
    static constexpr double ELLIPSE_BASE_RADIUS_Y = 50.0;
};

} // namespace slideEditor::model

#endif // SHAPE_HPP