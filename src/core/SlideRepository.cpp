#include "../../include/core/SlideRepository.hpp"
#include <iostream>

void SlideRepository::addSlide(std::unique_ptr<Slide> slide) {
    slides_.push_back(std::move(slide));
}

Slide* SlideRepository::getSlideById(int id) {
    for (auto& slide : slides_) {
        if (slide->getId() == id) return slide.get();
    }
 
    return nullptr;
}

const std::vector<std::unique_ptr<Slide>>& SlideRepository::getAllSlides() const {
    return slides_;
}

void SlideRepository::displayAll() const {
    for (const auto& slide : slides_) {
        std::cout << slide->toString() << "-------------------\n";
    }
}