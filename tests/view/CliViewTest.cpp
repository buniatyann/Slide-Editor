#include <gtest/gtest.h>
#include "view/cli/CliView.hpp"
#include "model/SlideRepository.hpp"
#include "model/SlideFactory.hpp"
#include <sstream>
#include <memory>

using namespace slideEditor::view;
using namespace slideEditor::model;

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
};

// ========================================
// Test IView Interface Methods ONLY
// ========================================

TEST_F(CliViewTest, DisplayMessage_PrintsMessage) {
    view_->displayMessage("Test message");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Test message"), std::string::npos);
}

TEST_F(CliViewTest, DisplayMessage_EmptyString_PrintsNewline) {
    view_->displayMessage("");
    
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
}

TEST_F(CliViewTest, DisplayMessage_MultipleLines) {
    view_->displayMessage("Line 1");
    view_->displayMessage("Line 2");
    view_->displayMessage("Line 3");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Line 1"), std::string::npos);
    EXPECT_NE(output.find("Line 2"), std::string::npos);
    EXPECT_NE(output.find("Line 3"), std::string::npos);
}

TEST_F(CliViewTest, DisplayError_PrintsError) {
    view_->displayError("Test error");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("[ERROR]"), std::string::npos);
    EXPECT_NE(output.find("Test error"), std::string::npos);
}

TEST_F(CliViewTest, DisplayError_MultipleErrors) {
    view_->displayError("Error 1");
    view_->displayError("Error 2");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Error 1"), std::string::npos);
    EXPECT_NE(output.find("Error 2"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPrompt_PrintsPrompt) {
    view_->displayPrompt();
    
    std::string output = getOutput();
    EXPECT_NE(output.find(">"), std::string::npos);
}

TEST_F(CliViewTest, DisplayPrompt_MultipleCalls) {
    view_->displayPrompt();
    view_->displayPrompt();
    view_->displayPrompt();
    
    std::string output = getOutput();
    // Should have multiple prompts
    size_t count = 0;
    size_t pos = 0;
    while ((pos = output.find(">", pos)) != std::string::npos) {
        count++;
        pos++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(CliViewTest, DisplaySlides_EmptyRepository_PrintsMessage) {
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("0 slide"), std::string::npos);
}

TEST_F(CliViewTest, DisplaySlides_NullRepository_HandlesGracefully) {
    // Should not crash
    view_->displaySlides(nullptr);
    
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
}

TEST_F(CliViewTest, DisplaySlides_SingleSlide_ShowsSlideData) {
    auto slide = SlideFactory::createSlide(0, "TestTitle", "TestContent", "TestTheme");
    repository_.addSlide(std::move(slide));
    
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("TestTitle"), std::string::npos);
    EXPECT_NE(output.find("TestContent"), std::string::npos);
    EXPECT_NE(output.find("TestTheme"), std::string::npos);
}

TEST_F(CliViewTest, DisplaySlides_MultipleSlides_ShowsAllSlides) {
    repository_.addSlide(SlideFactory::createSlide(0, "Slide1", "Content1", "Theme1"));
    repository_.addSlide(SlideFactory::createSlide(0, "Slide2", "Content2", "Theme2"));
    repository_.addSlide(SlideFactory::createSlide(0, "Slide3", "Content3", "Theme3"));
    
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Slide1"), std::string::npos);
    EXPECT_NE(output.find("Slide2"), std::string::npos);
    EXPECT_NE(output.find("Slide3"), std::string::npos);
}

TEST_F(CliViewTest, DisplaySlides_SlideWithShapes_ShowsShapeCount) {
    auto slide = SlideFactory::createSlide(0, "Title", "Content", "Theme");
    auto* slidePtr = slide.get();
    repository_.addSlide(std::move(slide));
    
    slidePtr->addShape(SlideFactory::createShape("circle", 1.0));
    slidePtr->addShape(SlideFactory::createShape("rectangle", 1.0));
    
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Shapes"), std::string::npos);
}

TEST_F(CliViewTest, DisplaySlides_ShowsSlideCount) {
    repository_.addSlide(SlideFactory::createSlide(0, "S1", "C1", "T1"));
    repository_.addSlide(SlideFactory::createSlide(0, "S2", "C2", "T2"));
    
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("2 slide"), std::string::npos);
}

TEST_F(CliViewTest, DisplayHelp_ShowsHelpText) {
    view_->displayHelp("This is help text");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("This is help text"), std::string::npos);
}

TEST_F(CliViewTest, DisplayHelp_EmptyString_PrintsSomething) {
    view_->displayHelp("");
    
    std::string output = getOutput();
    // Should at least print newlines
    EXPECT_FALSE(output.empty());
}

TEST_F(CliViewTest, DisplayHelp_MultilineHelp) {
    std::string helpText = "Command: create\n"
                          "Usage: create <title> <content> <theme>\n"
                          "Description: Creates a new slide";
    
    view_->displayHelp(helpText);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Command: create"), std::string::npos);
    EXPECT_NE(output.find("Usage:"), std::string::npos);
    EXPECT_NE(output.find("Description:"), std::string::npos);
}

TEST_F(CliViewTest, MixedMethodCalls_WorkCorrectly) {
    view_->displayMessage("Starting application");
    view_->displayPrompt();
    view_->displayError("Sample error");
    view_->displayMessage("Continuing...");
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Starting application"), std::string::npos);
    EXPECT_NE(output.find(">"), std::string::npos);
    EXPECT_NE(output.find("[ERROR]"), std::string::npos);
    EXPECT_NE(output.find("Sample error"), std::string::npos);
    EXPECT_NE(output.find("Continuing..."), std::string::npos);
}

TEST_F(CliViewTest, DisplaySlides_ComplexSlides_FormatsCorrectly) {
    auto slide1 = SlideFactory::createSlide(0, "Introduction", "Welcome to SlideEditor", "Modern");
    auto* s1Ptr = slide1.get();
    s1Ptr->addShape(SlideFactory::createShape("circle", 1.5));
    s1Ptr->addShape(SlideFactory::createShape("rectangle", 2.0));
    repository_.addSlide(std::move(slide1));
    
    auto slide2 = SlideFactory::createSlide(0, "Conclusion", "Thank you", "Classic");
    repository_.addSlide(std::move(slide2));
    
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_NE(output.find("Introduction"), std::string::npos);
    EXPECT_NE(output.find("Conclusion"), std::string::npos);
    EXPECT_NE(output.find("Modern"), std::string::npos);
    EXPECT_NE(output.find("Classic"), std::string::npos);
}

TEST_F(CliViewTest, AllMethodsWork_Integration) {
    // Test that all 5 IView methods work
    view_->displayMessage("Message works");
    view_->displayError("Error works");
    view_->displayPrompt();
    view_->displayHelp("Help works");
    
    auto slide = SlideFactory::createSlide(0, "Test", "Test", "Test");
    repository_.addSlide(std::move(slide));
    view_->displaySlides(&repository_);
    
    std::string output = getOutput();
    EXPECT_FALSE(output.empty());
    EXPECT_NE(output.find("Message works"), std::string::npos);
    EXPECT_NE(output.find("Error works"), std::string::npos);
    EXPECT_NE(output.find(">"), std::string::npos);
    EXPECT_NE(output.find("Help works"), std::string::npos);
    EXPECT_NE(output.find("Test"), std::string::npos);
}