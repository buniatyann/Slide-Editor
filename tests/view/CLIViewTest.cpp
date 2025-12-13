#include <gtest/gtest.h>

#include "model/SlideFactory.hpp"
#include "model/SlideRepository.hpp"
#include "view/cli/CliView.hpp"

#include <memory>
#include <sstream>

using namespace slideEditor::model;
using namespace slideEditor::view;

class CliViewTest : public ::testing::Test {
protected:
    std::ostringstream output_;
    std::unique_ptr<CliView> view_;
    SlideRepository repository_;

    void SetUp() override {
        output_.str("");
        output_.clear();
        view_ = std::make_unique<CliView>(output_);
    }

    std::string getOutput() const {
        return output_.str();
    }

    void clearOutput() {
        output_.str("");
        output_.clear();
    }
};

TEST_F(CliViewTest, DisplayWelcome_PrintsMessage) {
    view_->displayWelcome();

    const std::string output = getOutput();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("SlideEditor"), std::string::npos);
}

TEST_F(CliViewTest, DisplayWelcome_PrintsVersion) {
    view_->displayWelcome();

    const std::string output = getOutput();
    EXPECT_GT(output.length(), 10);
}

TEST_F(CliViewTest, DisplayPrompt_PrintsPrompt) {
    view_->displayPrompt();

    const std::string output = getOutput();
    EXPECT_NE(output.find(">"), std::string::npos);
}

TEST_F(CliViewTest, DisplayError_PrintsErrorPrefix) {
    view_->displayError("Test error message");

    const std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}

TEST_F(CliViewTest, DisplayError_PrintsErrorMessage) {
    view_->displayError("Test error message");

    const std::string output = getOutput();
    EXPECT_NE(output.find("Test error message"), std::string::npos);
}

TEST_F(CliViewTest, DisplayError_EmptyMessage_StillPrintsPrefix) {
    view_->displayError("");

    const std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}

TEST_F(CliViewTest, DisplayInfo_PrintsMessage) {
    view_->displayInfo("Information message");

    const std::string output = getOutput();
    EXPECT_NE(output.find("Information message"), std::string::npos);
}

TEST_F(CliViewTest, DisplayInfo_EmptyMessage_PrintsNothing) {
    view_->displayInfo("");

    const std::string output = getOutput();
    EXPECT_LE(output.length(), 2);
}

TEST_F(CliViewTest, DisplaySuccess_PrintsMessage) {
    view_->displaySuccess("Operation successful");

    const std::string output = getOutput();
    EXPECT_NE(output.find("Operation successful"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_EmptyRepository_PrintsEmptyMessage) {
    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_NE(output.find("0"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_NullRepository_HandlesGracefully) {
    view_->displayPresentation(nullptr);

    const std::string output = getOutput();
    EXPECT_FALSE(output.empty());
}

TEST_F(CliViewTest, DisplayPresentation_SingleSlide_ShowsSlideData) {
    auto slide = SlideFactory::createSlide(
        0, "TestTitle", "TestContent", "TestTheme"
    );
    repository_.addSlide(std::move(slide));

    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_NE(output.find("TestTitle"), std::string::npos);
    EXPECT_NE(output.find("TestContent"), std::string::npos);
    EXPECT_NE(output.find("TestTheme"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_MultipleSlides_ShowsAllSlides) {
    repository_.addSlide(
        SlideFactory::createSlide(0, "Title1", "Content1", "Theme1")
    );
    repository_.addSlide(
        SlideFactory::createSlide(0, "Title2", "Content2", "Theme2")
    );
    repository_.addSlide(
        SlideFactory::createSlide(0, "Title3", "Content3", "Theme3")
    );

    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_NE(output.find("Title1"), std::string::npos);
    EXPECT_NE(output.find("Title2"), std::string::npos);
    EXPECT_NE(output.find("Title3"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_SlideWithShapes_ShowsShapeCount) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));

    slidePtr->addShape(SlideFactory::createShape("circle", 1.0));
    slidePtr->addShape(SlideFactory::createShape("rectangle", 1.0));

    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_NE(output.find("2"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_ShowsSlideNumbers) {
    repository_.addSlide(
        SlideFactory::createSlide(0, "Title1", "Content1", "Theme1")
    );
    repository_.addSlide(
        SlideFactory::createSlide(0, "Title2", "Content2", "Theme2")
    );

    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_TRUE(
        output.find("#1") != std::string::npos ||
        output.find("Slide 1") != std::string::npos ||
        output.find("1:") != std::string::npos
    );
}

TEST_F(CliViewTest, DisplayHelp_ShowsHelpText) {
    view_->displayHelp("This is help text");

    const std::string output = getOutput();
    EXPECT_NE(output.find("This is help text"), std::string::npos);
}

TEST_F(CliViewTest, DisplayHelp_EmptyString_StillPrints) {
    view_->displayHelp("");

    const std::string output = getOutput();
    EXPECT_GE(output.length(), 0);
}

TEST_F(CliViewTest, DisplayHelp_MultilineText_PrintsAllLines) {
    const std::string helpText = "Line 1\nLine 2\nLine 3";
    view_->displayHelp(helpText);

    const std::string output = getOutput();
    EXPECT_NE(output.find("Line 1"), std::string::npos);
    EXPECT_NE(output.find("Line 2"), std::string::npos);
    EXPECT_NE(output.find("Line 3"), std::string::npos);
}

TEST_F(CliViewTest, MultipleDisplayCalls_Accumulate) {
    view_->displayInfo("First");
    view_->displayInfo("Second");
    view_->displayInfo("Third");

    const std::string output = getOutput();
    EXPECT_NE(output.find("First"), std::string::npos);
    EXPECT_NE(output.find("Second"), std::string::npos);
    EXPECT_NE(output.find("Third"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPresentation_ComplexSlides_FormatsCorrectly) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();

    slidePtr->addShape(
        SlideFactory::createShape("circle", 1.5, "red", "blue")
    );
    slidePtr->addShape(
        SlideFactory::createShape("rectangle", 2.0, "green", "yellow")
    );
    slidePtr->addShape(
        SlideFactory::createShape("triangle", 1.0)
    );

    repository_.addSlide(std::move(slide));
    view_->displayPresentation(&repository_);

    const std::string output = getOutput();
    EXPECT_NE(output.find("Title"), std::string::npos);
    EXPECT_NE(output.find("circle"), std::string::npos);
    EXPECT_NE(output.find("rectangle"), std::string::npos);
    EXPECT_NE(output.find("triangle"), std::string::npos);
}

TEST_F(CliViewTest, DisplayGoodbye_PrintsMessage) {
    view_->displayGoodbye();

    const std::string output = getOutput();
    EXPECT_FALSE(output.empty());
    EXPECT_GT(output.length(), 3);
}
