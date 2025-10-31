#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "interfaces/IShape.hpp"
#include "model/BoundingBox.hpp"

namespace slideEditor::model {

class Ellipse : public core::IShape {
public:
    explicit Ellipse(double scale = 1.0);
    
    core::ShapeType getType() const override;
    double getScale() const override;
    std::string toString() const override;
    void setScale(double scale) override;
    std::string serialize() const override;
    std::unique_ptr<core::IShape> clone() const override;
    
    double getRadiusX() const;
    double getRadiusY() const;
    BoundingBox getBoundingBox() const;

private:
    double scale_;
    static constexpr double BASE_RADIUS_X = 1.5;
    static constexpr double BASE_RADIUS_Y = 1.0;
};

} // namespace slideEditor::model

#endif // ELLIPSE_HPP