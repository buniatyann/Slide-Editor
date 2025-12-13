#include <gtest/gtest.h>

#include "controller/CommandController.hpp"
#include "model/SlideRepository.hpp"
#include "serialization/JsonSerializer.hpp"
#include "view/cli/CliView.hpp"

#include <fstream>
#include <memory>
#include <sstream>

using namespace slideEditor;

class EndToEndTest : public ::testing::Test {
protected:
    std::ostringstream viewOutput_;
    std::shared_ptr<model::SlideRepository> repository_;
    std::shared_ptr<serialization::JsonSerializer> serializer_;
    std::shared_ptr<view::CliView> view_;
    std::unique_ptr<controller::CommandController> controller_;

    void SetUp() override {
        viewOutput_.str("");

        repository_ = std::make_shared<model::SlideRepository>();
        serializer_ = std::make_shared<serialization::JsonSerializer>();
        view_ = std::make_shared<view::CliView>(viewOutput_);

        controller_ = std::make_unique<controller::CommandController>(
            repository_, serializer_, view_
        );
    }

    void TearDown() override {
        std::remove("test_e2e.json");
        std::remove("test_e2e.svg");
    }

    bool executeCommand(const std::string& cmd) {
        viewOutput_.str("");
        return controller_->processCommandLine(cmd);
    }

    std::string getOutput() const {
        return viewOutput_.str();
    }
};

TEST_F(EndToEndTest, CreateSlide_Success) {
    bool result = executeCommand("create TestTitle TestContent TestTheme");

    EXPECT_TRUE(result);
    EXPECT_EQ(repository_->getSlideCount(), 1);

    std::string output = getOutput();
    EXPECT_NE(output.find("created"), std::string::npos);
}

TEST_F(EndToEndTest, CreateMultipleSlides_Success) {
    executeCommand("create Title1 Content1 Theme1");
    executeCommand("create Title2 Content2 Theme2");
    executeCommand("create Title3 Content3 Theme3");

    EXPECT_EQ(repository_->getSlideCount(), 3);
}

TEST_F(EndToEndTest, CreateAndAddShape_Success) {
    executeCommand("create TestTitle TestContent TestTheme");
    bool result = executeCommand("addshape 1 circle 1.5");

    EXPECT_TRUE(result);

    auto* slide = repository_->getSlide(1);
    ASSERT_NE(slide, nullptr);
    EXPECT_EQ(slide->getShapeCount(), 1);
}

TEST_F(EndToEndTest, CreateAndAddMultipleShapes_Success) {
    executeCommand("create TestTitle TestContent TestTheme");
    executeCommand("addshape 1 circle 1.0");
    executeCommand("addshape 1 rectangle 1.5");
    executeCommand("addshape 1 triangle 2.0");

    auto* slide = repository_->getSlide(1);
    EXPECT_EQ(slide->getShapeCount(), 3);
}

TEST_F(EndToEndTest, CreateAddShapeAndUndo_Success) {
    executeCommand("create TestTitle TestContent TestTheme");
    executeCommand("addshape 1 circle 1.5");

    auto* slide = repository_->getSlide(1);
    EXPECT_EQ(slide->getShapeCount(), 1);

    bool result = executeCommand("undo");

    EXPECT_TRUE(result);
    EXPECT_EQ(slide->getShapeCount(), 0);
}

TEST_F(EndToEndTest, UndoAndRedo_Success) {
    executeCommand("create TestTitle TestContent TestTheme");
    EXPECT_EQ(repository_->getSlideCount(), 1);

    executeCommand("undo");
    EXPECT_EQ(repository_->getSlideCount(), 0);

    bool result = executeCommand("redo");

    EXPECT_TRUE(result);
    EXPECT_EQ(repository_->getSlideCount(), 1);
}

TEST_F(EndToEndTest, MultipleUndoRedo_Success) {
    executeCommand("create S1 C1 T1");
    executeCommand("create S2 C2 T2");
    executeCommand("create S3 C3 T3");

    EXPECT_EQ(repository_->getSlideCount(), 3);

    executeCommand("undo");
    executeCommand("undo");
    EXPECT_EQ(repository_->getSlideCount(), 1);

    executeCommand("redo");
    EXPECT_EQ(repository_->getSlideCount(), 2);
}

TEST_F(EndToEndTest, CreateAndDisplay_Success) {
    executeCommand("create TestTitle TestContent TestTheme");
    bool result = executeCommand("display");

    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("TestTitle"), std::string::npos);
    EXPECT_NE(output.find("TestContent"), std::string::npos);

    std::ifstream checkFile("test_e2e.json");
    EXPECT_TRUE(checkFile.good());
    checkFile.close();

    repository_->clear();
    EXPECT_EQ(repository_->getSlideCount(), 0);

    executeCommand("load test_e2e.json");
    EXPECT_EQ(repository_->getSlideCount(), 1);

    auto* slide = repository_->getAllSlides().front().get();
    EXPECT_EQ(slide->getTitle(), "TestTitle");
}

TEST_F(EndToEndTest, CreateMultipleSlidesWithShapes_Success) {
    executeCommand("create Slide1 Content1 Theme1");
    executeCommand("addshape 1 circle 1.0 red blue");
    executeCommand("addshape 1 rectangle 1.5 green yellow");
    executeCommand("create Slide2 Content2 Theme2");
    executeCommand("addshape 2 triangle 2.0");

    bool result = executeCommand("display");

    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("Slide1"), std::string::npos);
    EXPECT_NE(output.find("Slide2"), std::string::npos);
    EXPECT_NE(output.find("circle"), std::string::npos);
    EXPECT_NE(output.find("triangle"), std::string::npos);
}

TEST_F(EndToEndTest, ComplexWorkflow_CreateSaveLoadModify) {
    executeCommand("create Title1 Content1 Theme1");
    executeCommand("addshape 1 circle 1.0");
    executeCommand("save test_e2e.json");

    executeCommand("create Title2 Content2 Theme2");
    EXPECT_EQ(repository_->getSlideCount(), 2);

    executeCommand("load test_e2e.json");

    EXPECT_EQ(repository_->getSlideCount(), 1);

    auto* slide = repository_->getSlide(1);
    EXPECT_EQ(slide->getTitle(), "Title1");
    EXPECT_EQ(slide->getShapeCount(), 1);
}

TEST_F(EndToEndTest, RemoveShape_Success) {
    executeCommand("create Test Test Test");
    executeCommand("addshape 1 circle 1.0");
    executeCommand("addshape 1 rectangle 1.0");

    auto* slide = repository_->getSlide(1);
    EXPECT_EQ(slide->getShapeCount(), 2);

    executeCommand("removeshape 1 0");
    EXPECT_EQ(slide->getShapeCount(), 1);
}

TEST_F(EndToEndTest, RemoveShapeAndUndo_Success) {
    executeCommand("create Test Test Test");
    executeCommand("addshape 1 circle 1.0");
    executeCommand("removeshape 1 0");

    auto* slide = repository_->getSlide(1);
    EXPECT_EQ(slide->getShapeCount(), 0);

    executeCommand("undo");
    EXPECT_EQ(slide->getShapeCount(), 1);
}

TEST_F(EndToEndTest, ErrorHandling_InvalidCommand) {
    bool result = executeCommand("invalidcommand");
    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}

TEST_F(EndToEndTest, ErrorHandling_MissingArguments) {
    bool result = executeCommand("create Title");
    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("Missing"), std::string::npos);
}

TEST_F(EndToEndTest, ErrorHandling_InvalidSlideId) {
    bool result = executeCommand("addshape 999 circle 1.0");
    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}

TEST_F(EndToEndTest, ErrorHandling_InvalidShapeType) {
    executeCommand("create Test Test Test");
    bool result = executeCommand("addshape 1 invalidsshape 1.0");

    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("ERROR"), std::string::npos);
}

TEST_F(EndToEndTest, Help_ShowsCommands) {
    bool result = executeCommand("help");
    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("create"), std::string::npos);
    EXPECT_NE(output.find("addshape"), std::string::npos);
}

TEST_F(EndToEndTest, HelpSpecific_ShowsCommandHelp) {
    bool result = executeCommand("help create");
    EXPECT_TRUE(result);

    std::string output = getOutput();
    EXPECT_NE(output.find("create"), std::string::npos);
}

TEST_F(EndToEndTest, ExitCommand_StopsExecution) {
    bool result = executeCommand("exit");
    EXPECT_FALSE(result);
}

TEST_F(EndToEndTest, CompleteSession_MultipleOperations) {
    executeCommand("create Slide1 Content1 Theme1");
    executeCommand("addshape 1 circle 1.5 red blue");
    executeCommand("addshape 1 rectangle 2.0");
    executeCommand("create Slide2 Content2 Theme2");
    executeCommand("addshape 2 triangle 1.0");

    executeCommand("save test_e2e.json");

    executeCommand("addshape 2 ellipse 1.5");
    executeCommand("undo");
    executeCommand("display");

    EXPECT_EQ(repository_->getSlideCount(), 2);

    auto* slide1 = repository_->getSlide(1);
    auto* slide2 = repository_->getSlide(2);

    EXPECT_EQ(slide1->getShapeCount(), 2);
    EXPECT_EQ(slide2->getShapeCount(), 1);
}
