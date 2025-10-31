#include "model/shapes/Triangle.hpp"
#include <sstream>
#include <iomanip>
#include <cmath> 

namespace slideEditor::model {

Triangle::Triangle(double scale) : scale_(scale) {
    if (scale <= 0) {
        scale_ = 1.0;
    }
}

core::ShapeType Triangle::getType() const {
    return core::ShapeType::TRIANGLE;
}

double Triangle::getScale() const {
    return scale_;
}

std::string Triangle::toString() const {
    std::ostringstream oss;
    oss << "Triangle(side=" << std::fixed << std::setprecision(2) << getSideLength()
        << ", height=" << getHeight() << ")";

    return oss.str();
}

void Triangle::setScale(double scale) {
    if (scale > 0) {
        scale_ = scale;
    }
}

std::string Triangle::serialize() const {
    std::ostringstream oss;
    oss << R"({"type":"triangle","scale":)" << scale_ << "}";
    
    return oss.str();
}

std::unique_ptr<core::IShape> Triangle::clone() const {
    return std::make_unique<Triangle>(scale_);
}

double Triangle::getSideLength() const {
    return BASE_SIDE * scale_;
}

double Triangle::getHeight() const {
    return (std::sqrt(3.0) / 2.0) * getSideLength();
}

BoundingBox Triangle::getBoundingBox() const {
    double side = getSideLength();
    double height = getHeight();
    
    // Center the bounding box around origin
    return BoundingBox(-side / 2.0, -height / 2.0, side, height);
}

} // namespace slideEditor::model
