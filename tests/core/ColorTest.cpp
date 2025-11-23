#include <gtest/gtest.h>
#include "model/Color.hpp"

using namespace slideEditor::model;

class ColorTest : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(ColorTest, DefaultConstructor_CreatesBlack) {
    Color color;
    
    EXPECT_EQ(color.r, 0);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    EXPECT_EQ(color.a, 255);
}

TEST_F(ColorTest, ParameterizedConstructor_SetsValues) {
    Color color(255, 128, 64, 200);
    
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 128);
    EXPECT_EQ(color.b, 64);
    EXPECT_EQ(color.a, 200);
}

TEST_F(ColorTest, NamedColors_CreateCorrectValues) {
    Color red = Color::Red();
    EXPECT_EQ(red.r, 255);
    EXPECT_EQ(red.g, 0);
    EXPECT_EQ(red.b, 0);
    
    Color green = Color::Green();
    EXPECT_EQ(green.r, 0);
    EXPECT_EQ(green.g, 255);
    EXPECT_EQ(green.b, 0);
    
    Color blue = Color::Blue();
    EXPECT_EQ(blue.r, 0);
    EXPECT_EQ(blue.g, 0);
    EXPECT_EQ(blue.b, 255);
}

TEST_F(ColorTest, ToHex_ConvertsCorrectly) {
    Color red(255, 0, 0);
    EXPECT_EQ(red.toHex(), "#ff0000");
    
    Color white(255, 255, 255);
    EXPECT_EQ(white.toHex(), "#ffffff");
    
    Color custom(18, 52, 86);
    EXPECT_EQ(custom.toHex(), "#123456");
}

TEST_F(ColorTest, GetOpacity_ReturnsCorrectValue) {
    Color opaque(0, 0, 0, 255);
    EXPECT_DOUBLE_EQ(opaque.getOpacity(), 1.0);
    
    Color halfTransparent(0, 0, 0, 127);
    EXPECT_NEAR(halfTransparent.getOpacity(), 0.498, 0.01);
    
    Color transparent(0, 0, 0, 0);
    EXPECT_DOUBLE_EQ(transparent.getOpacity(), 0.0);
}

TEST_F(ColorTest, FromString_ParsesNamedColors) {
    EXPECT_EQ(Color::fromString("red").toHex(), Color::Red().toHex());
    EXPECT_EQ(Color::fromString("RED").toHex(), Color::Red().toHex());
    EXPECT_EQ(Color::fromString("green").toHex(), Color::Green().toHex());
    EXPECT_EQ(Color::fromString("blue").toHex(), Color::Blue().toHex());
}

TEST_F(ColorTest, FromString_ParsesHexColors) {
    Color color = Color::fromString("#ff0000");
    EXPECT_EQ(color.r, 255);
    EXPECT_EQ(color.g, 0);
    EXPECT_EQ(color.b, 0);
    
    Color shortHex = Color::fromString("#f0a");
    EXPECT_EQ(shortHex.r, 255);
    EXPECT_EQ(shortHex.g, 0);
    EXPECT_EQ(shortHex.b, 170);
}

TEST_F(ColorTest, FromString_ReturnsBlackForInvalid) {
    Color invalid = Color::fromString("notacolor");
    EXPECT_EQ(invalid.toHex(), Color::Black().toHex());
}