#ifndef SLIDEEDITOR_CORE_SLIDE_FACTORY_HPP
#define SLIDEEDITOR_CORE_SLIDE_FACTORY_HPP

#include <memory>
#include "Slide.hpp"
#include <nlohmann/json.hpp>

class SlideFactory {
private:
    int nextId_ = 1;
public:
    std::unique_ptr<Slide> createSlide(const std::string& title, const std::string& content, const std::string& theme);
    std::unique_ptr<Slide> fromJson(const nlohmann::json& j, int& maxId);
    void resetId(int startId);
};

#endif // SLIDEEDITOR_CORE_SLIDE_FACTORY_HPP