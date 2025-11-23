#include <gtest/gtest.h>
#include "model/Slide.hpp"
#include "model/shapes/Shape.hpp"

using namespace slideEditor::model;
using namespace slideEditor::core;

class SlideTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(SlideTest, Constructor_SetsProperties) {
    Slide slide(1, "Title", "Content", "Theme");
    
    EXPECT_EQ(slide.getId(), 1);
    EXPECT_EQ(slide.getTitle(), "Title");
    EXPECT_EQ(slide.getContent(), "Content");
    EXPECT_EQ(slide.getTheme(), "Theme");
    EXPECT_EQ(slide.getShapeCount(), 0);
}

TEST_F(SlideTest, AddShape_IncreasesCount) {
    Slide slide(1, "Title", "Content", "Theme");
    
    auto shape = std::make_unique<Shape>(ShapeType::CIRCLE);
    slide.addShape(std::move(shape));
    
    EXPECT_EQ(slide.getShapeCount(), 1);
}

TEST_F(SlideTest, AddShape_IgnoresNull) {
    Slide slide(1, "Title", "Content", "Theme");
    
    slide.addShape(nullptr);
    
    EXPECT_EQ(slide.getShapeCount(), 0);
}

TEST_F(SlideTest, RemoveShape_DecreasesCount) {
    Slide slide(1, "Title", "Content", "Theme");
    
    slide.addShape(std::make_unique<Shape>(ShapeType::CIRCLE));
    slide.addShape(std::make_unique<Shape>(ShapeType::RECTANGLE));
    
    bool removed = slide.removeShape(0);
    
    EXPECT_TRUE(removed);
    EXPECT_EQ(slide.getShapeCount(), 1);
}

TEST_F(SlideTest, RemoveShape_ReturnsFalseForInvalidIndex) {
    Slide slide(1, "Title", "Content", "Theme");
    
    slide.addShape(std::make_unique<Shape>(ShapeType::CIRCLE));
    
    bool removed = slide.removeShape(5);
    
    EXPECT_FALSE(removed);
    EXPECT_EQ(slide.getShapeCount(), 1);
}

TEST_F(SlideTest, GetShapes_ReturnsConstReference) {
    Slide slide(1, "Title", "Content", "Theme");
    
    slide.addShape(std::make_unique<Shape>(ShapeType::CIRCLE));
    slide.addShape(std::make_unique<Shape>(ShapeType::RECTANGLE));
    
    const auto& shapes = slide.getShapes();
    
    EXPECT_EQ(shapes.size(), 2);
    EXPECT_EQ(shapes[0]->getType(), ShapeType::CIRCLE);
    EXPECT_EQ(shapes[1]->getType(), ShapeType::RECTANGLE);
}

TEST_F(SlideTest, Serialize_ReturnsJSON) {
    Slide slide(1, "Title", "Content", "Theme");
    slide.addShape(std::make_unique<Shape>(ShapeType::CIRCLE));
    
    std::string json = slide.serialize();
    
    EXPECT_NE(json.find("\"id\":1"), std::string::npos);
    EXPECT_NE(json.find("\"title\":\"Title\""), std::string::npos);
    EXPECT_NE(json.find("\"shapes\":["), std::string::npos);
}