#include "model/shapes/Circle.hpp"
#include <sstream>
#include <iomanip>

namespace slideEditor::model {

Circle::Circle(double scale) : scale_(scale) {
    if (scale <= 0) { 
        scale_ = 1.0;
    }
}

core::ShapeType Circle::getType() const {
    return core::ShapeType::CIRCLE;
}

double Circle::getScale() const {
    return scale_;
}

std::string Circle::toString() const {
    std::ostringstream oss;
    oss << "Circle(radius=" << std::fixed << std::setprecision(2) 
        << getRadius() << ")";
    
    return oss.str();
}

void Circle::setScale(double scale) {
    if (scale > 0) {
        scale_ = scale;
    }
}

std::string Circle::serialize() const {
    std::ostringstream oss;
    oss << R"({"type":"circle","scale":)" << scale_ << "}";
    
    return oss.str();
}

std::unique_ptr<core::IShape> Circle::clone() const {
    return std::make_unique<Circle>(scale_);
}

double Circle::getRadius() const {
    return BASE_RADIUS * scale_;
}

BoundingBox Circle::getBoundingBox() const {
    double r = getRadius();
    return BoundingBox(-r, -r, 2*r, 2*r);
}

} // namespace slideEditor::model