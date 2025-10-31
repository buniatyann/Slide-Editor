#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "interfaces/IShape.hpp"
#include "model/BoundingBox.hpp"

namespace slideEditor::model {

class Triangle : public core::IShape {
public:
    explicit Triangle(double scale = 1.0);
    
    core::ShapeType getType() const override;
    double getScale() const override;
    std::string toString() const override;
    void setScale(double scale) override;
    std::string serialize() const override;
    std::unique_ptr<core::IShape> clone() const override;
    
    double getSideLength() const;
    double getHeight() const;
    BoundingBox getBoundingBox() const;

private:
    double scale_;
    static constexpr double BASE_SIDE = 1.0; 
};

} // namespace slideEditor::model

#endif // TRIANGLE_HPP