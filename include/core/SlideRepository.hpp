#ifndef SLIDEEDITOR_CORE_SLIDEREPOSITORY_HPP
#define SLIDEEDITOR_CORE_SLIDEREPOSITORY_HPP

#include "../interfaces/ISlideRepository.hpp"
#include <vector>
#include <memory>

class SlideRepository : public ISlideRepository {
public:
    void addSlide(std::unique_ptr<Slide> slide) override;
    Slide* getSlideById(int id) override;
    void displayAll() const override;
    nlohmann::json toJson() const override;
    void clear() override;
    void moveSlide(std::size_t fromIndex, std::size_t toIndex) override;

private:
    std::vector<std::unique_ptr<Slide>> slides_;
};

#endif // SLIDEEDITOR_CORE_SLIDEREPOSITORY_HPP