#include "serialization/JsonSerializer.hpp"
#include "model/SlideFactory.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace slideEditor::serialization {

bool JsonSerializer::save(const core::ISlideRepository* repository, 
                          const std::string& filename) {
    if (!repository) {
        lastError_ = "Repository is null";
        return false;
    }
    
    try {
        std::string jsonStr = serializeRepository(repository);
        std::ofstream file(filename);
        if (!file.is_open()) {
            lastError_ = "Cannot open file for writing: " + filename;
            return false;
        }
        
        file << jsonStr;
        file.close();
        lastError_.clear();
        
        return true;
        
    } catch (const std::exception& e) {
        lastError_ = std::string("Serialization error: ") + e.what();
        return false;
    }
}

bool JsonSerializer::load(core::ISlideRepository* repository, 
                          const std::string& filename) {
    if (!repository) {
        lastError_ = "Repository is null";
        return false;
    }
    
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            lastError_ = "Cannot open file for reading: " + filename;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        std::string jsonStr = buffer.str();
        repository->clear();
        bool success = deserializeToRepository(repository, jsonStr);
        if (!success) {
            return false;
        }
        
        lastError_.clear();
        return true;
        
    } catch (const std::exception& e) {
        lastError_ = std::string("Deserialization error: ") + e.what();
        return false;
    }
}

std::string JsonSerializer::getLastError() const {
    return lastError_;
}

std::string JsonSerializer::serializeRepository(
    const core::ISlideRepository* repo) 
{
    json root = json::object();
    root["version"] = "1.0";
    root["slides"] = json::array();
    const auto& slides = repo->getAllSlides();
    for (const auto& slide : slides) {
        json slideObj = json::object();
        slideObj["id"] = slide->getId();
        slideObj["title"] = slide->getTitle();
        slideObj["content"] = slide->getContent();
        slideObj["theme"] = slide->getTheme();
        slideObj["shapes"] = json::array();
        const auto& shapes = slide->getShapes();
        for (const auto& shape : shapes) {
            json shapeObj = json::object();
            switch (shape->getType()) {
                case core::ShapeType::CIRCLE:
                    shapeObj["type"] = "circle";
                    break;
                case core::ShapeType::RECTANGLE:
                    shapeObj["type"] = "rectangle";
                    break;
                case core::ShapeType::TRIANGLE:
                    shapeObj["type"] = "triangle";
                    break;
                case core::ShapeType::ELLIPSE:
                    shapeObj["type"] = "ellipse";
                    break;
            }
            
            shapeObj["scale"] = shape->getScale();
            slideObj["shapes"].push_back(shapeObj);
        }
        
        root["slides"].push_back(slideObj);
    }
    
    return root.dump(2);  // Pretty print with 2-space indent
}

bool JsonSerializer::deserializeToRepository(
    core::ISlideRepository* repo,
    const std::string& jsonStr) 
{
    try {
        json root = json::parse(jsonStr);
        
        if (!root.contains("slides") || !root["slides"].is_array()) {
            lastError_ = "Invalid JSON format: missing 'slides' array";
            return false;
        }
        
        for (const auto& slideJson : root["slides"]) {
            std::string title = slideJson.value("title", "");
            std::string content = slideJson.value("content", "");
            std::string theme = slideJson.value("theme", "");
            
            auto slide = model::SlideFactory::createSlide(
                0,  // ID will be assigned by repository
                title,
                content,
                theme
            );
            
            if (slideJson.contains("shapes") && slideJson["shapes"].is_array()) {
                for (const auto& shapeJson : slideJson["shapes"]) {
                    std::string type = shapeJson.value("type", "");
                    double scale = shapeJson.value("scale", 1.0);
                    auto shape = model::SlideFactory::createShape(type, scale);
                    if (shape) {
                        slide->addShape(std::move(shape));
                    }
                }
            }
            
            repo->addSlide(std::move(slide));
        }
        
        return true;        
    } catch (const json::exception& e) {
        lastError_ = std::string("JSON parsing error: ") + e.what();
        return false;
    }
}

} // namespace slideEditor::serialization