#include <gtest/gtest.h>
#include "model/SlideFactory.hpp"
#include "model/Color.hpp"  

using namespace slideEditor::model;
using namespace slideEditor::core;

class SlideFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(SlideFactoryTest, CreateShape_Circle_CreatesValidShape) {
    auto shape = SlideFactory::createShape("circle", 1.5);
    
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getType(), ShapeType::CIRCLE);
    EXPECT_DOUBLE_EQ(shape->getScale(), 1.5);
}

TEST_F(SlideFactoryTest, CreateShape_Rectangle_CreatesValidShape) {
    auto shape = SlideFactory::createShape("rectangle", 2.0);
    
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getType(), ShapeType::RECTANGLE);
    EXPECT_DOUBLE_EQ(shape->getScale(), 2.0);
}

TEST_F(SlideFactoryTest, CreateShape_Triangle_CreatesValidShape) {
    auto shape = SlideFactory::createShape("triangle", 1.0);
    
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getType(), ShapeType::TRIANGLE);
}

TEST_F(SlideFactoryTest, CreateShape_Ellipse_CreatesValidShape) {
    auto shape = SlideFactory::createShape("ellipse", 1.3);
    
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getType(), ShapeType::ELLIPSE);
}

TEST_F(SlideFactoryTest, CreateShape_CaseInsensitive) {
    auto shape1 = SlideFactory::createShape("CIRCLE", 1.0);
    auto shape2 = SlideFactory::createShape("Circle", 1.0);
    auto shape3 = SlideFactory::createShape("circle", 1.0);
    
    EXPECT_NE(shape1, nullptr);
    EXPECT_NE(shape2, nullptr);
    EXPECT_NE(shape3, nullptr);
}

TEST_F(SlideFactoryTest, CreateShape_WithColors_SetsColors) {
    auto shape = SlideFactory::createShape("circle", 1.0, "red", "blue");
    
    ASSERT_NE(shape, nullptr);
    EXPECT_EQ(shape->getBorderColor(), Color::Red().toHex());
    EXPECT_EQ(shape->getFillColor(), Color::Blue().toHex());
}

TEST_F(SlideFactoryTest, CreateShape_InvalidType_ReturnsNull) {
    auto shape = SlideFactory::createShape("invalid", 1.0);
    
    EXPECT_EQ(shape, nullptr);
}

TEST_F(SlideFactoryTest, CreateSlide_CreatesValidSlide) {
    auto slide = SlideFactory::createSlide(5, "Title", "Content", "Theme");
    
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getTitle(), "Title");
    EXPECT_EQ(slide->getContent(), "Content");
    EXPECT_EQ(slide->getTheme(), "Theme");
}

TEST_F(SlideFactoryTest, IsValidShapeType_ReturnsTrueForValid) {
    EXPECT_TRUE(SlideFactory::isValidShapeType("circle"));
    EXPECT_TRUE(SlideFactory::isValidShapeType("rectangle"));
    EXPECT_TRUE(SlideFactory::isValidShapeType("triangle"));
    EXPECT_TRUE(SlideFactory::isValidShapeType("ellipse"));
}

TEST_F(SlideFactoryTest, IsValidShapeType_CaseInsensitive) {
    EXPECT_TRUE(SlideFactory::isValidShapeType("CIRCLE"));
    EXPECT_TRUE(SlideFactory::isValidShapeType("Circle"));
}

TEST_F(SlideFactoryTest, IsValidShapeType_ReturnsFalseForInvalid) {
    EXPECT_FALSE(SlideFactory::isValidShapeType("invalid"));
    EXPECT_FALSE(SlideFactory::isValidShapeType("square"));
}