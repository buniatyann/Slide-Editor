#include "../include/model/BoundingBox.hpp"

namespace slideEditor::model {

BoundingBox::BoundingBox(double x, double y, double w, double h)
    : x(x), y(y), width(w), height(h) {}

double BoundingBox::area() const {
    return width * height;
}

bool BoundingBox::contains(double px, double py) const {
    return px >= x && px <= x + width && 
           py >= y && py <= y + height;
}

bool BoundingBox::intersects(const BoundingBox& other) const {
    return !(x + width < other.x || other.x + other.width < x ||
             y + height < other.y || other.y + other.height < y);
}

} // namespace slideEditor::model