#ifndef SLIDEEDITOR_CORE_SHAPE_HPP
#define SLIDEEDITOR_CORE_SHAPE_HPP

#include <string>
#include <memory>
#include "../interfaces/IShape.hpp"

class Triangle : public IShape {
private:
    float scale_;
public:
    explicit Triangle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;
};

class Circle : public IShape {
private:
    float scale_;
public:
    explicit Circle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;
};

class Rectangle : public IShape {
private:
    float scale_;
public:
    explicit Rectangle(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;
};

class Ellipse : public IShape {
private:
    float scale_;
public:
    explicit Ellipse(float scale = 1.0f);
    std::string getType() const override;
    float getScale() const override;
    void setScale(float scale) override;
    nlohmann::json toJson() const override;
    std::string toString() const override;
};

class ShapeFactory {
public:
    static std::unique_ptr<IShape> createShape(const std::string& type, float scale = 1.0f);
    static std::unique_ptr<IShape> fromJson(const nlohmann::json& j);
};

#endif // SLIDEEDITOR_CORE_SHAPE_HPP