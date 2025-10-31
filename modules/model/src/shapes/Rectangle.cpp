#include "model/shapes/Rectangle.hpp"
#include <sstream>
#include <iomanip>

namespace slideEditor::model {

Rectangle::Rectangle(double scale) : scale_(scale) {
    if (scale <= 0) {
        scale_ = 1.0;
    }
}

core::ShapeType Rectangle::getType() const {
    return core::ShapeType::RECTANGLE;
}

double Rectangle::getScale() const {
    return scale_;
}

std::string Rectangle::toString() const {
    std::ostringstream oss;
    oss << "Rectangle(width=" << std::fixed << std::setprecision(2) << getWidth()
        << ", height=" << getHeight() << ")";
        
    return oss.str();
}

void Rectangle::setScale(double scale) {
    if (scale > 0) {
        scale_ = scale;
    }
}

std::string Rectangle::serialize() const {
    std::ostringstream oss;
    oss << R"({"type":"rectangle","scale":)" << scale_ << "}";
    
    return oss.str();
}

std::unique_ptr<core::IShape> Rectangle::clone() const {
    return std::make_unique<Rectangle>(scale_);
}

double Rectangle::getWidth() const {
    return BASE_WIDTH * scale_;
}

double Rectangle::getHeight() const {
    return BASE_HEIGHT * scale_;
}

BoundingBox Rectangle::getBoundingBox() const {
    double w = getWidth();
    double h = getHeight();
    
    return BoundingBox(-w / 2.0, -h / 2.0, w, h);
}

} // namespace slideEditor::model
