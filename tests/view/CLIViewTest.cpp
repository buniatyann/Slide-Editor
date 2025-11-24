#include <gtest/gtest.h>

#include "view/cli/CliView.hpp"
#include "model/SlideRepository.hpp"
#include "model/Slide.hpp"
#include "model/shapes/Shape.hpp"
#include "model/Color.hpp"

#include <memory>
#include <sstream>
#include <string>

using slideEditor::core::ShapeType;
using slideEditor::model::Color;
using slideEditor::model::Shape;
using slideEditor::model::Slide;
using slideEditor::model::SlideRepository;

class CliViewTest : public ::testing::Test {
protected:
    std::ostringstream buffer;
    slideEditor::view::CliView view{buffer};
    SlideRepository repository;

    static std::unique_ptr<Slide> makeSlide(
        int id,
        const std::string& title,
        const std::string& content,
        const std::string& theme) {
        return std::make_unique<Slide>(id, title, content, theme);
    }

    std::string output() const {
        return buffer.str();
    }
};

TEST_F(CliViewTest, DisplayMessageWritesLine) {
    view.displayMessage("Hello CLI");

    EXPECT_EQ(output(), "Hello CLI\n");
}

TEST_F(CliViewTest, DisplayErrorPrefixesWithErrorTag) {
    view.displayError("Something went wrong");

    EXPECT_EQ(output(), "[ERROR] Something went wrong\n");
}

TEST_F(CliViewTest, DisplayPromptWritesPromptWithoutNewline) {
    view.displayPrompt();

    EXPECT_EQ(output(), "> ");
}

TEST_F(CliViewTest, DisplayHelpAddsBlankLineAndText) {
    view.displayHelp("Usage: help");

    EXPECT_EQ(output(), "\nUsage: help\n");
}

TEST_F(CliViewTest, DisplaySlidesWithNullRepositoryShowsError) {
    view.displaySlides(nullptr);

    EXPECT_EQ(output(), "[ERROR] Repository not available\n");
}

TEST_F(CliViewTest, DisplaySlidesShowsMessageWhenEmpty) {
    view.displaySlides(&repository);

    EXPECT_EQ(output(), "No slides in presentation.\n");
}

TEST_F(CliViewTest, DisplaySlidesPrintsSlidesAndShapes) {
    auto intro = makeSlide(1, "Intro", "Welcome", "Light");
    intro->addShape(std::make_unique<Shape>(ShapeType::CIRCLE, 1.5, Color::Red(), Color::Blue()));
    repository.addSlide(std::move(intro));

    auto roadmap = makeSlide(2, "Roadmap", "Next steps", "Dark");
    roadmap->addShape(std::make_unique<Shape>(ShapeType::RECTANGLE, 1.0, Color::Green(), Color::Yellow()));
    roadmap->addShape(std::make_unique<Shape>(ShapeType::TRIANGLE, 0.8, Color::Purple(), Color::Orange()));
    repository.addSlide(std::move(roadmap));

    view.displaySlides(&repository);
    auto text = output();

    EXPECT_NE(text.find("PRESENTATION (2 slide(s))"), std::string::npos);
    EXPECT_NE(text.find("Slide #1"), std::string::npos);
    EXPECT_NE(text.find("Title: Intro"), std::string::npos);
    EXPECT_NE(text.find("Theme: Light"), std::string::npos);
    EXPECT_NE(text.find("[0] circle"), std::string::npos);
    EXPECT_NE(text.find("Slide #2"), std::string::npos);
    EXPECT_NE(text.find("[1] triangle"), std::string::npos);
}

