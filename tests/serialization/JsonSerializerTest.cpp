#include <gtest/gtest.h>
#include "serialization/JsonSerializer.hpp"
#include "model/SlideRepository.hpp"
#include "model/SlideFactory.hpp"
#include "model/Slide.hpp"
#include <fstream>
#include <sstream>

using namespace slideEditor::serialization;
using namespace slideEditor::model;

class JsonSerializerTest : public ::testing::Test {
protected:
    JsonSerializer serializer_;
    SlideRepository repository_;
    std::string testFile_ = "test_serialization.json";
    
    void SetUp() override {
        // Clean up any existing test file
        std::remove(testFile_.c_str());
    }
    
    void TearDown() override {
        // Clean up test file
        std::remove(testFile_.c_str());
    }
    
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.good()) 
            return "";
        
        std::ostringstream ss;
        ss << file.rdbuf();
        
        return ss.str();
    }
};

TEST_F(JsonSerializerTest, Save_EmptyRepository_CreatesFile) {
    bool success = serializer_.save(&repository_, testFile_);
    
    EXPECT_TRUE(success);
    EXPECT_TRUE(fileExists(testFile_));
}

TEST_F(JsonSerializerTest, Save_EmptyRepository_CreatesValidJSON) {
    serializer_.save(&repository_, testFile_);
    
    std::string content = readFile(testFile_);
    
    EXPECT_NE(content.find("{"), std::string::npos);
    EXPECT_NE(content.find("}"), std::string::npos);
    EXPECT_NE(content.find("slides"), std::string::npos);
}

TEST_F(JsonSerializerTest, Save_SingleSlide_ContainsSlideData) {
    auto slide = SlideFactory::createSlide(0, "TestTitle", "TestContent", "TestTheme");
    repository_.addSlide(std::move(slide));
    
    bool success = serializer_.save(&repository_, testFile_);
    
    EXPECT_TRUE(success);
    
    std::string content = readFile(testFile_);
    EXPECT_NE(content.find("TestTitle"), std::string::npos);
    EXPECT_NE(content.find("TestContent"), std::string::npos);
    EXPECT_NE(content.find("TestTheme"), std::string::npos);
}

TEST_F(JsonSerializerTest, Save_MultipleSlides_ContainsAllSlides) {
    repository_.addSlide(SlideFactory::createSlide(0, "Title1", "Content1", "Theme1"));
    repository_.addSlide(SlideFactory::createSlide(0, "Title2", "Content2", "Theme2"));
    repository_.addSlide(SlideFactory::createSlide(0, "Title3", "Content3", "Theme3"));
    
    serializer_.save(&repository_, testFile_);
    
    std::string content = readFile(testFile_);
    EXPECT_NE(content.find("Title1"), std::string::npos);
    EXPECT_NE(content.find("Title2"), std::string::npos);
    EXPECT_NE(content.find("Title3"), std::string::npos);
}

TEST_F(JsonSerializerTest, Save_SlideWithShapes_ContainsShapeData) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.5, "red", "blue"));
    slidePtr->addShape(SlideFactory::createShape("rectangle", 2.0));
    
    serializer_.save(&repository_, testFile_);
    
    std::string content = readFile(testFile_);
    EXPECT_NE(content.find("circle"), std::string::npos);
    EXPECT_NE(content.find("rectangle"), std::string::npos);
    EXPECT_NE(content.find("1.5"), std::string::npos);
}

TEST_F(JsonSerializerTest, Save_InvalidPath_ReturnsFalse) {
    bool success = serializer_.save(&repository_, "/invalid/path/file.json");
    
    EXPECT_FALSE(success);
}

TEST_F(JsonSerializerTest, Save_NullRepository_ReturnsFalse) {
    bool success = serializer_.save(nullptr, testFile_);
    
    EXPECT_FALSE(success);
}

TEST_F(JsonSerializerTest, Load_ValidFile_ReturnsTrue) {
    repository_.addSlide(SlideFactory::createSlide(0, "Title", "Content", "Theme"));
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    bool success = serializer_.load(&newRepo, testFile_);
    
    EXPECT_TRUE(success);
}

TEST_F(JsonSerializerTest, Load_ValidFile_RestoresSlideCount) {
    repository_.addSlide(SlideFactory::createSlide(0, "Title1", "Content1", "Theme1"));
    repository_.addSlide(SlideFactory::createSlide(0, "Title2", "Content2", "Theme2"));
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    EXPECT_EQ(newRepo.getSlideCount(), 2);
}

TEST_F(JsonSerializerTest, Load_ValidFile_RestoresSlideData) {
    auto slide = SlideFactory::createSlide(0, "TestTitle", "TestContent", "TestTheme");
    repository_.addSlide(std::move(slide));
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    ASSERT_EQ(newRepo.getSlideCount(), 1);
    
    const auto& slides = newRepo.getAllSlides();
    auto* loadedSlide = slides.front().get();
    
    EXPECT_EQ(loadedSlide->getTitle(), "TestTitle");
    EXPECT_EQ(loadedSlide->getContent(), "TestContent");
    EXPECT_EQ(loadedSlide->getTheme(), "TestTheme");
}

TEST_F(JsonSerializerTest, Load_WithShapes_RestoresShapes) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.5, "red", "blue"));
    slidePtr->addShape(SlideFactory::createShape("rectangle", 2.0));
    
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    auto* loadedSlide = newRepo.getAllSlides().front().get();
    EXPECT_EQ(loadedSlide->getShapeCount(), 2);
}

TEST_F(JsonSerializerTest, Load_NonexistentFile_ReturnsFalse) {
    SlideRepository newRepo;
    
    bool success = serializer_.load(&newRepo, "nonexistent_file.json");
    
    EXPECT_FALSE(success);
}

TEST_F(JsonSerializerTest, Load_NullRepository_ReturnsFalse) {
    repository_.addSlide(SlideFactory::createSlide(0, "Title", "Content", "Theme"));
    serializer_.save(&repository_, testFile_);
    
    bool success = serializer_.load(nullptr, testFile_);
    
    EXPECT_FALSE(success);
}

TEST_F(JsonSerializerTest, Load_InvalidJSON_ReturnsFalse) {
    // Create invalid JSON file
    std::ofstream file(testFile_);
    file << "This is not valid JSON {[}]";
    file.close();
    
    SlideRepository newRepo;
    bool success = serializer_.load(&newRepo, testFile_);
    
    EXPECT_FALSE(success);
}

TEST_F(JsonSerializerTest, RoundTrip_SimpleSlide_PreservesData) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    repository_.addSlide(std::move(slide));
    
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    EXPECT_EQ(newRepo.getSlideCount(), repository_.getSlideCount());
    
    auto* original = repository_.getAllSlides().front().get();
    auto* loaded = newRepo.getAllSlides().front().get();
    
    EXPECT_EQ(loaded->getTitle(), original->getTitle());
    EXPECT_EQ(loaded->getContent(), original->getContent());
    EXPECT_EQ(loaded->getTheme(), original->getTheme());
}

TEST_F(JsonSerializerTest, RoundTrip_ComplexPresentation_PreservesAll) {
    // Create complex presentation
    auto slide1 = SlideFactory::createSlide(0, "S1", "C1", "T1");
    auto* s1Ptr = slide1.get();
    s1Ptr->addShape(SlideFactory::createShape("circle", 1.5, "red", "blue"));
    s1Ptr->addShape(SlideFactory::createShape("rectangle", 2.0, "green", "yellow"));
    repository_.addSlide(std::move(slide1));
    
    auto slide2 = SlideFactory::createSlide(0, "S2", "C2", "T2");
    auto* s2Ptr = slide2.get();
    s2Ptr->addShape(SlideFactory::createShape("triangle", 1.0));
    repository_.addSlide(std::move(slide2));
    
    auto slide3 = SlideFactory::createSlide(0, "S3", "C3", "T3");
    repository_.addSlide(std::move(slide3));
    
    // Save and load
    serializer_.save(&repository_, testFile_);
    
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    // Verify
    EXPECT_EQ(newRepo.getSlideCount(), 3);
    
    auto& slides = newRepo.getAllSlides();
    EXPECT_EQ(slides[0]->getShapeCount(), 2);
    EXPECT_EQ(slides[1]->getShapeCount(), 1);
    EXPECT_EQ(slides[2]->getShapeCount(), 0);
}

TEST_F(JsonSerializerTest, Load_ClearsExistingData) {
    // Add data to new repo
    SlideRepository newRepo;
    newRepo.addSlide(SlideFactory::createSlide(0, "Old", "Old", "Old"));
    EXPECT_EQ(newRepo.getSlideCount(), 1);
    
    // Create and save different data
    repository_.addSlide(SlideFactory::createSlide(0, "New", "New", "New"));
    serializer_.save(&repository_, testFile_);
    
    // Load should clear old data
    serializer_.load(&newRepo, testFile_);
    
    EXPECT_EQ(newRepo.getSlideCount(), 1);
    auto* slide = newRepo.getAllSlides().front().get();
    EXPECT_EQ(slide->getTitle(), "New");
}

TEST_F(JsonSerializerTest, Save_Overwrite_ReplacesFile) {
    // Save first version
    repository_.addSlide(SlideFactory::createSlide(0, "Version1", "V1", "V1"));
    serializer_.save(&repository_, testFile_);
    
    // Save second version
    repository_.clear();
    repository_.addSlide(SlideFactory::createSlide(0, "Version2", "V2", "V2"));
    serializer_.save(&repository_, testFile_);
    
    // Load should get second version
    SlideRepository newRepo;
    serializer_.load(&newRepo, testFile_);
    
    auto* slide = newRepo.getAllSlides().front().get();
    EXPECT_EQ(slide->getTitle(), "Version2");
}

TEST_F(JsonSerializerTest, EmptyFile_LoadFails) {
    // Create empty file
    std::ofstream file(testFile_);
    file.close();
    
    SlideRepository newRepo;
    bool success = serializer_.load(&newRepo, testFile_);
    
    EXPECT_FALSE(success);
}