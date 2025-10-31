#include "model/Slide.hpp"
#include <sstream>

namespace slideEditor::model {

Slide::Slide(int id, std::string title, std::string content, std::string theme)
    : id_(id), title_(std::move(title)), content_(std::move(content)), 
      theme_(std::move(theme)) {}

int Slide::getId() const { 
    return id_;
}

std::string Slide::getTitle() const { 
    return title_; 
}

std::string Slide::getContent() const { 
    return content_; 
}

std::string Slide::getTheme() const { 
    return theme_; 
}

const std::vector<std::unique_ptr<core::IShape>>& Slide::getShapes() const {
    return shapes_;
}

void Slide::addShape(std::unique_ptr<core::IShape> shape) {
    if (shape) {
        shapes_.push_back(std::move(shape));
    }
}

bool Slide::removeShape(size_t index) {
    if (index < shapes_.size()) {
        shapes_.erase(shapes_.begin() + index);
        return true;
    }

    return false;
}

size_t Slide::getShapeCount() const {
    return shapes_.size();
}

std::string Slide::serialize() const {
    std::ostringstream oss;
    oss << R"({"id":)" << id_ 
        << R"(,"title":")" << title_ 
        << R"(","content":")" << content_
        << R"(","theme":")" << theme_
        << R"(","shapes":[)";
    
    for (size_t i = 0; i < shapes_.size(); ++i) {
        if (i > 0) oss << ",";
        oss << shapes_[i]->serialize();
    }

    oss << "]}";
    return oss.str();
}

} // namespace slideEditor::model