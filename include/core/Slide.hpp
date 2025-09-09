#ifndef SLIDEEDITOR_CORE_SLIDE_HPP
#define SLIDEEDITOR_CORE_SLIDE_HPP

#include <vector>
#include <string>
#include <memory>
#include "../interfaces/IShape.hpp"
#include <nlohmann/json.hpp>

struct Text {
    std::string content;
    float size = 1.0f;
    std::string font = "Arial"; // Default font
    std::string color = "Black"; // Default color
    float lineWidth = 1.0f; // Default line width

    nlohmann::json toJson() const;
    std::string toString() const;
};

class Slide {    
public:
    Slide(int id, const std::string& title, const std::string& content, const std::string& theme);
    int getId() const;
    std::string getTitle() const;
    std::string getContent() const;
    std::string getTheme() const;
    void addText(const Text& text);
    void addShape(std::unique_ptr<IShape> shape);
    void removeText(std::size_t index);
    void removeShape(std::size_t index);
    const std::vector<Text>& getTexts() const;
    const std::vector<std::unique_ptr<IShape>>& getShapes() const;
    nlohmann::json toJson() const;
    std::string toString() const;

private:
    int id_;
    std::string title_;
    std::string content_;
    std::string theme_;
    std::vector<Text> texts_;
    std::vector<std::unique_ptr<IShape>> shapes_;
};

#endif // SLIDEEDITOR_CORE_SLIDE_HPP