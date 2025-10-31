#include "model/shapes/Ellipse.hpp"
#include <sstream>
#include <iomanip>

namespace slideEditor::model {

Ellipse::Ellipse(double scale) : scale_(scale) {
    if (scale <= 0) {
        scale_ = 1.0;
    }
}

core::ShapeType Ellipse::getType() const {
    return core::ShapeType::ELLIPSE;
}

double Ellipse::getScale() const {
    return scale_;
}

std::string Ellipse::toString() const {
    std::ostringstream oss;
    oss << "Ellipse(rx=" << std::fixed << std::setprecision(2) << getRadiusX()
        << ", ry=" << getRadiusY() << ")";

    return oss.str();
}

void Ellipse::setScale(double scale) {
    if (scale > 0) {
        scale_ = scale;
    }
}

std::string Ellipse::serialize() const {
    std::ostringstream oss;
    oss << R"({"type":"ellipse","scale":)" << scale_ << "}";
    
    return oss.str();
}

std::unique_ptr<core::IShape> Ellipse::clone() const {
    return std::make_unique<Ellipse>(scale_);
}

double Ellipse::getRadiusX() const {
    return BASE_RADIUS_X * scale_;
}

double Ellipse::getRadiusY() const {
    return BASE_RADIUS_Y * scale_;
}

BoundingBox Ellipse::getBoundingBox() const {
    double rx = getRadiusX();
    double ry = getRadiusY();
    
    return BoundingBox(-rx, -ry, 2 * rx, 2 * ry);
}

} // namespace slideEditor::model
