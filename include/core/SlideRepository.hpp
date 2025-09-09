#ifndef SLIDEEDITOR_CORE_SLIDE_REPOSITORY_HPP
#define SLIDEEDITOR_CORE_SLIDE_REPOSITORY_HPP

#include "../interfaces/ISlideRepository.hpp"

class SlideRepository : public ISlideRepository {
public:
    void addSlide(std::unique_ptr<Slide> slide) override;
    Slide* getSlideById(int id) override;
    const std::vector<std::unique_ptr<Slide>>& getAllSlides() const override;
    void displayAll() const override;
    
private:
    std::vector<std::unique_ptr<Slide>> slides_;
};

#endif // SLIDEEDITOR_CORE_SLIDE_REPOSITORY_HPP
