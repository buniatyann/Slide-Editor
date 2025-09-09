#include "../../include/serialization/JsonSerializer.hpp"
#include <fstream>
#include <iostream>

void JsonSerializer::save(const ISlideRepository& repo, const std::string& filepath) const {
    nlohmann::json j;
    j["language"] = "C++";
    nlohmann::json slidesJson = nlohmann::json::array();
    for (const auto& slide : repo.getAllSlides()) {
        slidesJson.push_back(slide->toJson());
    }
    
    j["slides"] = slidesJson;
    std::ofstream file(filepath);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
        std::cout << "Saved presentation to " << filepath << std::endl;
    } 
    else {
        std::cout << "Failed to open file: " << filepath << std::endl;
    }
}

void JsonSerializer::load(ISlideRepository& repo, SlideFactory& factory, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filepath << std::endl;
        return;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    int maxId = 1;
    if (j.contains("slides") && j["slides"].is_array()) {
        for (const auto& slideJson : j["slides"]) {
            repo.addSlide(factory.fromJson(slideJson, maxId));
        }
    
        factory.resetId(maxId);
        std::cout << "Loaded presentation from " << filepath << std::endl;
    } 
    else {
        std::cout << "Invalid JSON format in " << filepath << std::endl;
    }
}