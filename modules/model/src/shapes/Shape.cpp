#include "model/shapes/Shape.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace slideEditor::model {

Shape::Shape(core::ShapeType type, 
             double scale,
             const Color& borderColor,
             const Color& fillColor)
    : type_(type), scale_(scale > 0 ? scale : 1.0),
      borderColor_(borderColor), fillColor_(fillColor) {}

core::ShapeType Shape::getType() const {
    return type_;
}

double Shape::getScale() const {
    return scale_;
}

void Shape::setScale(double scale) {
    if (scale > 0) {
        scale_ = scale;
    }
}

std::string Shape::getBorderColor() const {
    return borderColor_.toHex();
}

std::string Shape::getFillColor() const {
    return fillColor_.toHex();
}

void Shape::setBorderColor(const std::string& color) {
    borderColor_ = Color::fromString(color);
}

void Shape::setFillColor(const std::string& color) {
    fillColor_ = Color::fromString(color);
}

std::string Shape::toString() const {
    std::ostringstream oss;
    oss << core::shapeTypeToString(type_) 
        << "(scale=" << std::fixed << std::setprecision(2) << scale_
        << ", border=" << borderColor_.toHex()
        << ", fill=" << fillColor_.toHex() << ")";

    return oss.str();
}

std::string Shape::serialize() const {
    std::ostringstream oss;
    oss << R"({"type":")" << core::shapeTypeToString(type_) << R"(",)"
        << R"("scale":)" << scale_ << ","
        << R"("borderColor":")" << borderColor_.toHex() << R"(",)"
        << R"("fillColor":")" << fillColor_.toHex() << R"("})";

    return oss.str();
}

std::unique_ptr<core::IShape> Shape::clone() const {
    return std::make_unique<Shape>(type_, scale_, borderColor_, fillColor_);
}

std::string Shape::toSVG(double x, double y) const {
    std::ostringstream svg;
    std::string fill = fillColor_.toHex();
    std::string stroke = borderColor_.toHex();
    double strokeWidth = 2.0;
    double opacity = fillColor_.getOpacity();
    switch (type_) {
        case core::ShapeType::CIRCLE: {
            double r = getCircleRadius();
            svg << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << r << "\" "
                << "fill=\"" << fill << "\" stroke=\"" << stroke << "\" "
                << "stroke-width=\"" << strokeWidth << "\" "
                << "fill-opacity=\"" << opacity << "\" />";

            break;
        }
        
        case core::ShapeType::RECTANGLE: {
            double w = getRectangleWidth();
            double h = getRectangleHeight();
            svg << "<rect x=\"" << (x - w/2) << "\" y=\"" << (y - h/2) << "\" "
                << "width=\"" << w << "\" height=\"" << h << "\" "
                << "fill=\"" << fill << "\" stroke=\"" << stroke << "\" "
                << "stroke-width=\"" << strokeWidth << "\" "
                << "fill-opacity=\"" << opacity << "\" />";

            break;
        }
        
        case core::ShapeType::TRIANGLE: {
            double side = getTriangleSide();
            double height = side * std::sqrt(3.0) / 2.0;
            double x1 = x;
            double y1 = y - height * 2.0 / 3.0;
            double x2 = x - side / 2.0;
            double y2 = y + height / 3.0;
            double x3 = x + side / 2.0;
            double y3 = y + height / 3.0;
            
            svg << "<polygon points=\"" 
                << x1 << "," << y1 << " "
                << x2 << "," << y2 << " "
                << x3 << "," << y3 << "\" "
                << "fill=\"" << fill << "\" stroke=\"" << stroke << "\" "
                << "stroke-width=\"" << strokeWidth << "\" "
                << "fill-opacity=\"" << opacity << "\" />";

            break;
        }
        
        case core::ShapeType::ELLIPSE: {
            double rx = getEllipseRadiusX();
            double ry = getEllipseRadiusY();
            svg << "<ellipse cx=\"" << x << "\" cy=\"" << y << "\" "
                << "rx=\"" << rx << "\" ry=\"" << ry << "\" "
                << "fill=\"" << fill << "\" stroke=\"" << stroke << "\" "
                << "stroke-width=\"" << strokeWidth << "\" "
                << "fill-opacity=\"" << opacity << "\" />";
                
            break;
        }
    }
    
    return svg.str();
}

double Shape::getCircleRadius() const {
    return CIRCLE_BASE_RADIUS * scale_;
}

double Shape::getRectangleWidth() const {
    return RECTANGLE_BASE_WIDTH * scale_;
}

double Shape::getRectangleHeight() const {
    return RECTANGLE_BASE_HEIGHT * scale_;
}

double Shape::getTriangleSide() const {
    return TRIANGLE_BASE_SIDE * scale_;
}

double Shape::getEllipseRadiusX() const {
    return ELLIPSE_BASE_RADIUS_X * scale_;
}

double Shape::getEllipseRadiusY() const {
    return ELLIPSE_BASE_RADIUS_Y * scale_;
}

} // namespace slideEditor::model