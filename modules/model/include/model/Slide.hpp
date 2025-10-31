#ifndef SLIDE_HPP
#define SLIDE_HPP

#include "interfaces/ISlide.hpp"
#include <vector>
#include <memory>
#include <string>

namespace slideEditor::model {

class Slide : public core::ISlide {
public:
    Slide(int id, std::string title, std::string content, std::string theme);
    
    int getId() const override;
    std::string getTitle() const override;
    std::string getContent() const override;
    std::string getTheme() const override;
    const std::vector<std::unique_ptr<core::IShape>>& getShapes() const override;
    
    void addShape(std::unique_ptr<core::IShape> shape) override;
    bool removeShape(size_t index) override;
    size_t getShapeCount() const override;
    
    std::string serialize() const override;

private:
    int id_;
    std::string title_;
    std::string content_;
    std::string theme_;
    std::vector<std::unique_ptr<core::IShape>> shapes_;
};

} // namespace slideEditor::model

#endif // SLIDE_HPP