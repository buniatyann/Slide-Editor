#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

namespace slideEditor::model {

struct BoundingBox {
    double x;
    double y;
    double width;
    double height;
    
    BoundingBox(double x = 0, double y = 0, double w = 0, double h = 0);
    
    double area() const;
    bool contains(double px, double py) const;
    bool intersects(const BoundingBox& other) const;
};

} // namespace slideEditor::model

#endif // BOUNDING_BOX_HPP