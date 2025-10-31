#include "model/SlideRepository.hpp"

namespace slideEditor::model {

SlideRepository::SlideRepository() : nextId_(1) {}

int SlideRepository::addSlide(std::unique_ptr<core::ISlide> slide) {
    if (!slide) return -1;
    
    int id = nextId_++;
    size_t index = slides_.size();
    idToIndex_[id] = index;
    slides_.push_back(std::move(slide));
    
    return id;
}

bool SlideRepository::removeSlide(int id) {
    auto it = idToIndex_.find(id);
    if (it == idToIndex_.end()) {
        return false;
    }
    
    size_t index = it->second;
    slides_.erase(slides_.begin() + index);
    idToIndex_.erase(it);
    // Rebuild index map (indices shifted)
    idToIndex_.clear();
    for (size_t i = 0; i < slides_.size(); ++i) {
        idToIndex_[slides_[i]->getId()] = i;
    }
    
    return true;
}

core::ISlide* SlideRepository::getSlide(int id) {
    auto it = idToIndex_.find(id);
    if (it == idToIndex_.end()) {
        return nullptr;
    }

    return slides_[it->second].get();
}

const core::ISlide* SlideRepository::getSlide(int id) const {
    auto it = idToIndex_.find(id);
    if (it == idToIndex_.end()) {
        return nullptr;
    }
    
    return slides_[it->second].get();
}

const std::vector<std::unique_ptr<core::ISlide>>& 
SlideRepository::getAllSlides() const {
    return slides_;
}

void SlideRepository::clear() {
    slides_.clear();
    idToIndex_.clear();
    nextId_ = 1;
}

size_t SlideRepository::getSlideCount() const {
    return slides_.size();
}

bool SlideRepository::exists(int id) const {
    return idToIndex_.find(id) != idToIndex_.end();
}

} // namespace slideEditor::model