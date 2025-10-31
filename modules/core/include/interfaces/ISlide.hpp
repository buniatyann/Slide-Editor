#ifndef I_SLIDE_HPP
#define I_SLIDE_HPP

#include "IShape.hpp"
#include <vector>
#include <string>
#include <memory>

namespace slideEditor::core {

class ISlide {
public:
    virtual ~ISlide() = default;
    
    virtual int getId() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getContent() const = 0;
    virtual std::string getTheme() const = 0;
    virtual const std::vector<std::unique_ptr<IShape>>& getShapes() const = 0;
    virtual void addShape(std::unique_ptr<IShape> shape) = 0;
    virtual bool removeShape(size_t index) = 0;
    virtual size_t getShapeCount() const = 0;
    virtual std::string serialize() const = 0;
};

} // namespace slideEditor::core

#endif // I_SLIDE_HPP