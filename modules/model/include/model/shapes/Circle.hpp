#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "interfaces/IShape.hpp"
#include "model/BoundingBox.hpp"

namespace slideEditor::model {

class Circle : public core::IShape {
public:
    explicit Circle(double scale = 1.0);

    core::ShapeType getType() const override;
    double getScale() const override;
    std::string toString() const override;
    void setScale(double scale) override;
    std::string serialize() const override;
    std::unique_ptr<core::IShape> clone() const override;

    double getRadius() const;
    BoundingBox getBoundingBox() const;
    
private:
    double scale_;
    static constexpr double BASE_RADIUS = 1.0;
};

} // namespace slideEditor::morel

#endif // CIRCLE_HPP