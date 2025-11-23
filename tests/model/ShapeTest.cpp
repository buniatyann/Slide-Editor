#include <gtest/gtest.h>
#include "model/shapes/Shape.hpp"
#include "model/Color.hpp"

using namespace slideEditor::model;
using namespace slideEditor::core;

class ShapeTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ShapeTest, Constructor_SetsDefaultValues) {
    Shape circle(ShapeType::CIRCLE);
    
    EXPECT_EQ(circle.getType(), ShapeType::CIRCLE);
    EXPECT_DOUBLE_EQ(circle.getScale(), 1.0);
    EXPECT_EQ(circle.getBorderColor(), Color::Black().toHex());
    EXPECT_EQ(circle.getFillColor(), Color::White().toHex());
}

TEST_F(ShapeTest, Constructor_SetsCustomValues) {
    Shape circle(ShapeType::CIRCLE, 2.5, Color::Red(), Color::Blue());
    
    EXPECT_EQ(circle.getType(), ShapeType::CIRCLE);
    EXPECT_DOUBLE_EQ(circle.getScale(), 2.5);
    EXPECT_EQ(circle.getBorderColor(), Color::Red().toHex());
    EXPECT_EQ(circle.getFillColor(), Color::Blue().toHex());
}

TEST_F(ShapeTest, SetScale_UpdatesScale) {
    Shape circle(ShapeType::CIRCLE);
    circle.setScale(3.0);
    
    EXPECT_DOUBLE_EQ(circle.getScale(), 3.0);
}

TEST_F(ShapeTest, SetScale_IgnoresNegative) {
    Shape circle(ShapeType::CIRCLE, 2.0);
    circle.setScale(-1.0);
    
    EXPECT_DOUBLE_EQ(circle.getScale(), 2.0);  // Unchanged
}

TEST_F(ShapeTest, SetColors_UpdatesColors) {
    Shape circle(ShapeType::CIRCLE);
    circle.setBorderColor("red");
    circle.setFillColor("blue");
    
    EXPECT_EQ(circle.getBorderColor(), Color::Red().toHex());
    EXPECT_EQ(circle.getFillColor(), Color::Blue().toHex());
}

TEST_F(ShapeTest, ToString_ReturnsCorrectFormat) {
    Shape circle(ShapeType::CIRCLE, 1.5, Color::Red(), Color::Blue());
    std::string str = circle.toString();
    
    EXPECT_NE(str.find("circle"), std::string::npos);
    EXPECT_NE(str.find("1.50"), std::string::npos);
}

TEST_F(ShapeTest, Serialize_ReturnsJSON) {
    Shape circle(ShapeType::CIRCLE, 1.5, Color::Red(), Color::Blue());
    std::string json = circle.serialize();
    
    EXPECT_NE(json.find("\"type\":\"circle\""), std::string::npos);
    EXPECT_NE(json.find("\"scale\":1.5"), std::string::npos);
}

TEST_F(ShapeTest, Clone_CreatesDeepCopy) {
    Shape original(ShapeType::CIRCLE, 2.0, Color::Red(), Color::Blue());
    auto cloned = original.clone();
    
    EXPECT_EQ(cloned->getType(), original.getType());
    EXPECT_DOUBLE_EQ(cloned->getScale(), original.getScale());
    EXPECT_EQ(cloned->getBorderColor(), original.getBorderColor());
    EXPECT_EQ(cloned->getFillColor(), original.getFillColor());
}

TEST_F(ShapeTest, CircleDimensions_ScaleCorrectly) {
    Shape circle(ShapeType::CIRCLE, 2.0);
    EXPECT_DOUBLE_EQ(circle.getCircleRadius(), 100.0);  // 50 * 2.0
}

TEST_F(ShapeTest, RectangleDimensions_ScaleCorrectly) {
    Shape rect(ShapeType::RECTANGLE, 1.5);
    EXPECT_DOUBLE_EQ(rect.getRectangleWidth(), 150.0);   // 100 * 1.5
    EXPECT_DOUBLE_EQ(rect.getRectangleHeight(), 90.0);   // 60 * 1.5
}

TEST_F(ShapeTest, ToSVG_GeneratesValidSVG) {
    Shape circle(ShapeType::CIRCLE, 1.0, Color::Red(), Color::Blue());
    std::string svg = circle.toSVG(100, 100);
    
    EXPECT_NE(svg.find("<circle"), std::string::npos);
    EXPECT_NE(svg.find("cx=\"100\""), std::string::npos);
    EXPECT_NE(svg.find("cy=\"100\""), std::string::npos);
}