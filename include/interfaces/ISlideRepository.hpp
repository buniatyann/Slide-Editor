#ifndef SLIDEEDITOR_INTERFACES_ISLIDE_REPOSITORY_HPP
#define SLIDEEDITOR_INTERFACES_ISLIDE_REPOSITORY_HPP

#include <vector>
#include <memory>
#include "../core/Slide.hpp"

class ISlideRepository {
public:
    virtual ~ISlideRepository() = default;
    virtual void addSlide(std::unique_ptr<Slide> slide) = 0;
    virtual Slide* getSlideById(int id) = 0;
    virtual const std::vector<std::unique_ptr<Slide>>& getAllSlides() const = 0;
    virtual void displayAll() const = 0;
};

#endif // SLIDEEDITOR_INTERFACES_ISLIDE_REPOSITORY_HPP