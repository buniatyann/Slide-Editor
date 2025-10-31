#ifndef I_SLIDE_REPOSITORY_HPP
#define I_SLIDE_REPOSITORY_HPP

#include "ISlide.hpp"
#include <memory>
#include <vector>
#include <optional>

namespace slideEditor::core {

class ISlideRepository {
public:
    virtual ~ISlideRepository() = default;
    
    virtual int addSlide(std::unique_ptr<ISlide> slide) = 0;
    virtual bool removeSlide(int id) = 0;
    virtual ISlide* getSlide(int id) = 0;
    virtual const ISlide* getSlide(int id) const = 0;
    virtual const std::vector<std::unique_ptr<ISlide>>& getAllSlides() const = 0;
    virtual void clear() = 0;
    virtual size_t getSlideCount() const = 0;
    virtual bool exists(int id) const = 0;
};

} // namespace slideEditor::core

#endif // I_SLIDE_REPOSITORY_HPP