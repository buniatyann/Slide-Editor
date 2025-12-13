#include <gtest/gtest.h>
#include "view/SvgGenerator.hpp"
#include "model/SlideRepository.hpp"
#include "model/SlideFactory.hpp"
#include <fstream>
#include <sstream>

using namespace slideEditor::view;
using namespace slideEditor::model;

class SvgGeneratorTest : public ::testing::Test {
protected:
    SlideRepository repository_;
    std::string testFile_ = "test_output.svg";
    
    void SetUp() override {
        std::remove(testFile_.c_str());
    }
    
    void TearDown() override {
        std::remove(testFile_.c_str());
    }
    
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.good()) return "";
        
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
};

TEST_F(SvgGeneratorTest, GenerateSVG_EmptyRepository_CreatesValidSVG) {
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_FALSE(svg.empty());
    EXPECT_NE(svg.find("<?xml"), std::string::npos);
    EXPECT_NE(svg.find("<svg"), std::string::npos);
    EXPECT_NE(svg.find("</svg>"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_EmptyRepository_HasCorrectNamespace) {
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("xmlns=\"http://www.w3.org/2000/svg\""), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_NullRepository_ReturnsEmptyString) {
    std::string svg = SvgGenerator::generateSVG(nullptr);
    
    EXPECT_TRUE(svg.empty());
}

TEST_F(SvgGeneratorTest, GenerateSVG_SingleSlide_IncludesSlideData) {
    auto slide = SlideFactory::createSlide(0, "TestTitle", "TestContent", "TestTheme");
    repository_.addSlide(std::move(slide));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("TestTitle"), std::string::npos);
    EXPECT_NE(svg.find("TestContent"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_MultipleSlides_IncludesAllSlides) {
    repository_.addSlide(SlideFactory::createSlide(0, "Slide1", "Content1", "Theme1"));
    repository_.addSlide(SlideFactory::createSlide(0, "Slide2", "Content2", "Theme2"));
    repository_.addSlide(SlideFactory::createSlide(0, "Slide3", "Content3", "Theme3"));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("Slide1"), std::string::npos);
    EXPECT_NE(svg.find("Slide2"), std::string::npos);
    EXPECT_NE(svg.find("Slide3"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_SlideWithCircle_IncludesCircleElement) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.0));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("<circle"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_SlideWithRectangle_IncludesRectElement) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("rectangle", 1.0));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("<rect"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_SlideWithTriangle_IncludesPolygonElement) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("triangle", 1.0));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("<polygon"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_SlideWithEllipse_IncludesEllipseElement) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("ellipse", 1.0));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("<ellipse"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_MultipleShapes_IncludesAllShapes) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.0));
    slidePtr->addShape(SlideFactory::createShape("rectangle", 1.0));
    slidePtr->addShape(SlideFactory::createShape("triangle", 1.0));
    slidePtr->addShape(SlideFactory::createShape("ellipse", 1.0));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("<circle"), std::string::npos);
    EXPECT_NE(svg.find("<rect"), std::string::npos);
    EXPECT_NE(svg.find("<polygon"), std::string::npos);
    EXPECT_NE(svg.find("<ellipse"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_ColoredShapes_IncludesColors) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.0, "red", "blue"));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    // Should contain color references (either named colors or hex codes)
    EXPECT_TRUE(
        svg.find("red") != std::string::npos || 
        svg.find("#ff0000") != std::string::npos
    );
}

TEST_F(SvgGeneratorTest, SaveToFile_CreatesFile) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    repository_.addSlide(std::move(slide));
    
    bool success = SvgGenerator::saveToFile(&repository_, testFile_);
    
    EXPECT_TRUE(success);
    EXPECT_TRUE(fileExists(testFile_));
}

TEST_F(SvgGeneratorTest, SaveToFile_NullRepository_ReturnsFalse) {
    bool success = SvgGenerator::saveToFile(nullptr, testFile_);
    
    EXPECT_FALSE(success);
    EXPECT_FALSE(fileExists(testFile_));
}

TEST_F(SvgGeneratorTest, SaveToFile_InvalidPath_ReturnsFalse) {
    bool success = SvgGenerator::saveToFile(&repository_, "/invalid/path/file.svg");
    
    EXPECT_FALSE(success);
}

TEST_F(SvgGeneratorTest, SaveToFile_ContainsValidSVG) {
    auto slide = SlideFactory::createSlide(0, "TestTitle", "TestContent", "TestTheme");
    repository_.addSlide(std::move(slide));
    
    SvgGenerator::saveToFile(&repository_, testFile_);
    
    std::string content = readFile(testFile_);
    
    EXPECT_NE(content.find("<?xml"), std::string::npos);
    EXPECT_NE(content.find("<svg"), std::string::npos);
    EXPECT_NE(content.find("</svg>"), std::string::npos);
    EXPECT_NE(content.find("TestTitle"), std::string::npos);
}

TEST_F(SvgGeneratorTest, SaveToFile_Overwrite_ReplacesContent) {
    // Save first version
    repository_.addSlide(SlideFactory::createSlide(0, "Version1", "V1", "V1"));
    SvgGenerator::saveToFile(&repository_, testFile_);
    
    std::string content1 = readFile(testFile_);
    EXPECT_NE(content1.find("Version1"), std::string::npos);
    
    // Save second version
    repository_.clear();
    repository_.addSlide(SlideFactory::createSlide(0, "Version2", "V2", "V2"));
    SvgGenerator::saveToFile(&repository_, testFile_);
    
    std::string content2 = readFile(testFile_);
    EXPECT_NE(content2.find("Version2"), std::string::npos);
    EXPECT_EQ(content2.find("Version1"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_HasProperDimensions) {
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    EXPECT_NE(svg.find("width="), std::string::npos);
    EXPECT_NE(svg.find("height="), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVG_ComplexPresentation_GeneratesCorrectly) {
    // Create complex presentation
    auto slide1 = SlideFactory::createSlide(0, "S1", "C1", "T1");
    auto* s1Ptr = slide1.get();
    s1Ptr->addShape(SlideFactory::createShape("circle", 1.5, "red", "blue"));
    s1Ptr->addShape(SlideFactory::createShape("rectangle", 2.0, "green", "yellow"));
    repository_.addSlide(std::move(slide1));
    
    auto slide2 = SlideFactory::createSlide(0, "S2", "C2", "T2");
    auto* s2Ptr = slide2.get();
    s2Ptr->addShape(SlideFactory::createShape("triangle", 1.0));
    s2Ptr->addShape(SlideFactory::createShape("ellipse", 1.3));
    repository_.addSlide(std::move(slide2));
    
    std::string svg = SvgGenerator::generateSVG(&repository_);
    
    // Verify structure
    EXPECT_NE(svg.find("<?xml"), std::string::npos);
    EXPECT_NE(svg.find("<svg"), std::string::npos);
    
    // Verify slides
    EXPECT_NE(svg.find("S1"), std::string::npos);
    EXPECT_NE(svg.find("S2"), std::string::npos);
    
    // Verify shapes
    EXPECT_NE(svg.find("<circle"), std::string::npos);
    EXPECT_NE(svg.find("<rect"), std::string::npos);
    EXPECT_NE(svg.find("<polygon"), std::string::npos);
    EXPECT_NE(svg.find("<ellipse"), std::string::npos);
}