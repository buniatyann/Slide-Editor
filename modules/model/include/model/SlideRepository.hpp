#ifndef SLIDE_REPOSITORY_HPP
#define SLIDE_REPOSITORY_HPP

#include "interfaces/ISlideRepository.hpp"
#include <vector>
#include <memory>
#include <map>

namespace slideEditor::model {

class SlideRepository : public core::ISlideRepository {
public:
    SlideRepository();
    
    int addSlide(std::unique_ptr<core::ISlide> slide) override;
    bool removeSlide(int id) override;
    core::ISlide* getSlide(int id) override;
    const core::ISlide* getSlide(int id) const override;
    
    const std::vector<std::unique_ptr<core::ISlide>>& getAllSlides() const override;
    void clear() override;
    size_t getSlideCount() const override;
    bool exists(int id) const override;

private:
    std::vector<std::unique_ptr<core::ISlide>> slides_;
    std::map<int, size_t> idToIndex_;  // Fast lookup
    int nextId_;
};

} // namespace slideEditor::model

#endif // SLIDE_REPOSITORY_HPP