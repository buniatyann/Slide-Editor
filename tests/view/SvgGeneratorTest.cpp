#include <gtest/gtest.h>

#include "view/SvgGenerator.hpp"
#include "model/SlideRepository.hpp"
#include "model/Slide.hpp"
#include "model/shapes/Shape.hpp"
#include "model/Color.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <utility>
#include <vector>

namespace {

using slideEditor::core::ShapeType;
using slideEditor::model::Color;
using slideEditor::model::Shape;

class RecordingShape : public slideEditor::core::IShape {
public:
    const std::vector<std::pair<double, double>>& positions() const {
        return positions_;
    }

    ShapeType getType() const override { return ShapeType::CIRCLE; }
    double getScale() const override { return 1.0; }
    void setScale(double) override {}
    std::string getBorderColor() const override { return "#000000"; }
    std::string getFillColor() const override { return "#ffffff"; }
    void setBorderColor(const std::string&) override {}
    void setFillColor(const std::string&) override {}
    std::string toString() const override { return "recording-shape"; }
    std::string serialize() const override { return "{}"; }
    std::unique_ptr<slideEditor::core::IShape> clone() const override {
        return std::make_unique<RecordingShape>(*this);
    }

    std::string toSVG(double x, double y) const override {
        positions_.emplace_back(x, y);
        std::ostringstream oss;
        oss << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"5\"/>";
        return oss.str();
    }

private:
    mutable std::vector<std::pair<double, double>> positions_;
};

}  // namespace

class SvgGeneratorTest : public ::testing::Test {
protected:
    slideEditor::view::SvgGenerator generator;
    slideEditor::model::SlideRepository repository;

    static std::unique_ptr<slideEditor::model::Slide> makeSlide(
        int id,
        const std::string& title,
        const std::string& content,
        const std::string& theme) {
        return std::make_unique<slideEditor::model::Slide>(id, title, content, theme);
    }
};

TEST_F(SvgGeneratorTest, GenerateSVGReturnsEmptyStringWhenRepositoryIsNull) {
    EXPECT_TRUE(generator.generateSVG(nullptr).empty());
}

TEST_F(SvgGeneratorTest, GenerateSVGReturnsPlaceholderWhenRepositoryIsEmpty) {
    std::string svg = generator.generateSVG(&repository);

    EXPECT_NE(svg.find("No slides to display"), std::string::npos);
    EXPECT_NE(svg.find("<svg xmlns=\"http://www.w3.org/2000/svg\""), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSVGIncludesSlidesAndShapes) {
    auto overview = makeSlide(1, "Quarterly Results", "Highlights", "Light");
    overview->addShape(std::make_unique<Shape>(ShapeType::CIRCLE, 1.0, Color::Red(), Color::Blue()));
    repository.addSlide(std::move(overview));

    auto roadmap = makeSlide(2, "Product Roadmap", "Upcoming milestones", "Dark");
    roadmap->addShape(std::make_unique<Shape>(ShapeType::RECTANGLE, 1.0, Color::Green(), Color::Yellow()));
    repository.addSlide(std::move(roadmap));

    std::string svg = generator.generateSVG(&repository);

    EXPECT_NE(svg.find("<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"1300\">"),
              std::string::npos);
    EXPECT_NE(svg.find("g id=\"slide-1\""), std::string::npos);
    EXPECT_NE(svg.find("Quarterly Results"), std::string::npos);
    EXPECT_NE(svg.find("Theme: Light"), std::string::npos);
    EXPECT_NE(svg.find("g id=\"slide-2\""), std::string::npos);
    EXPECT_NE(svg.find("Product Roadmap"), std::string::npos);
    EXPECT_NE(svg.find("<circle"), std::string::npos);
    EXPECT_NE(svg.find("<rect"), std::string::npos);
}

TEST_F(SvgGeneratorTest, GenerateSlideSVGPlacesShapesUsingGridSpacing) {
    auto slide = makeSlide(99, "Layout", "Grid verification", "Neutral");
    std::vector<RecordingShape*> placedShapes;

    for (int i = 0; i < 6; ++i) {
        auto shape = std::make_unique<RecordingShape>();
        placedShapes.push_back(shape.get());
        slide->addShape(std::move(shape));
    }

    std::string svg = generator.generateSlideSVG(slide.get(), 0);
    EXPECT_NE(svg.find("g id=\"slide-99\""), std::string::npos);

    ASSERT_EQ(placedShapes[0]->positions().size(), 1u);
    EXPECT_DOUBLE_EQ(placedShapes[0]->positions()[0].first, 100.0);
    EXPECT_DOUBLE_EQ(placedShapes[0]->positions()[0].second, 150.0);

    ASSERT_EQ(placedShapes[5]->positions().size(), 1u);
    EXPECT_DOUBLE_EQ(placedShapes[5]->positions()[0].first, 100.0);
    EXPECT_DOUBLE_EQ(placedShapes[5]->positions()[0].second, 270.0);  // wrapped to next row
}

TEST_F(SvgGeneratorTest, GenerateSlideSVGOffsetsBasedOnSlideNumber) {
    auto slide = makeSlide(7, "Offset", "Second slide", "Theme");
    auto shape = std::make_unique<RecordingShape>();
    auto* rawShape = shape.get();
    slide->addShape(std::move(shape));

    generator.generateSlideSVG(slide.get(), 1);

    ASSERT_EQ(rawShape->positions().size(), 1u);
    EXPECT_DOUBLE_EQ(rawShape->positions()[0].second, 800.0);  // 650 offset + 150 start
}

TEST_F(SvgGeneratorTest, GenerateAndSaveWritesSVGToDisk) {
    auto slide = makeSlide(1, "Export", "Persist to file", "Dark");
    slide->addShape(std::make_unique<Shape>(ShapeType::ELLIPSE, 1.2, Color::Blue(), Color::Cyan()));
    repository.addSlide(std::move(slide));

    auto targetPath = std::filesystem::temp_directory_path() / "svg_generator_test.svg";
    std::filesystem::remove(targetPath);

    ASSERT_TRUE(generator.generateAndSave(&repository, targetPath.string()));

    std::ifstream file(targetPath);
    ASSERT_TRUE(file.is_open());
    std::string contents((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    EXPECT_FALSE(contents.empty());
    EXPECT_NE(contents.find("g id=\"slide-1\""), std::string::npos);

    std::filesystem::remove(targetPath);
}

