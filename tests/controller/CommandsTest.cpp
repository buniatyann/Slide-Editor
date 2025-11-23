#include <gtest/gtest.h>
#include "controller/commands/Commands.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "model/SlideRepository.hpp"
#include "model/SlideFactory.hpp"
#include "serialization/JsonSerializer.hpp"
#include "view/cli/CliView.hpp"
#include "io/OutputStream.hpp"
#include <memory>
#include <sstream>
#include <fstream>

using namespace slideEditor::controller;
using namespace slideEditor;

class CommandsTest : public ::testing::Test {
protected:
    std::shared_ptr<model::SlideRepository> repository_;
    std::shared_ptr<serialization::JsonSerializer> serializer_;
    std::shared_ptr<view::CliView> view_;
    std::ostringstream viewOutput_;
    io::OutputStream output_;
    
    void SetUp() override {
        repository_ = std::make_shared<model::SlideRepository>();
        serializer_ = std::make_shared<serialization::JsonSerializer>();
        view_ = std::make_shared<view::CliView>(viewOutput_);
    }
};

TEST_F(CommandsTest, CreateCommand_CreatesSlide) {
    UndoableCreateCommand cmd(repository_, "Title", "Content", "Theme");
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
    EXPECT_TRUE(cmd.wasSuccessful());
    EXPECT_EQ(repository_->getSlideCount(), 1);
}

TEST_F(CommandsTest, CreateCommand_CanBeUndone) {
    UndoableCreateCommand cmd(repository_, "Title", "Content", "Theme");
    cmd.execute(output_);
    bool undone = cmd.undo();
    
    EXPECT_TRUE(undone);
    EXPECT_EQ(repository_->getSlideCount(), 0);
}

TEST_F(CommandsTest, AddShapeCommand_AddsShape) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    int id = repository_->addSlide(std::move(slide));
    UndoableAddShapeCommand cmd(repository_, id, "circle", 1.5, "red", "blue");
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
    
    auto* retrievedSlide = repository_->getSlide(id);
    EXPECT_EQ(retrievedSlide->getShapeCount(), 1);
}

TEST_F(CommandsTest, AddShapeCommand_FailsForInvalidSlide) {
    UndoableAddShapeCommand cmd(repository_, 999, "circle", 1.5);
    bool success = cmd.execute(output_);
    
    EXPECT_FALSE(success);
}

TEST_F(CommandsTest, AddShapeCommand_CanBeUndone) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    int id = repository_->addSlide(std::move(slide));
    UndoableAddShapeCommand cmd(repository_, id, "circle", 1.5);
    cmd.execute(output_);

    bool undone = cmd.undo();
    
    EXPECT_TRUE(undone);
    
    auto* retrievedSlide = repository_->getSlide(id);
    EXPECT_EQ(retrievedSlide->getShapeCount(), 0);
}

TEST_F(CommandsTest, RemoveShapeCommand_RemovesShape) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    int id = repository_->addSlide(std::move(slide));
    auto* slidePtr = repository_->getSlide(id);
    slidePtr->addShape(model::SlideFactory::createShape("circle", 1.0));
    UndoableRemoveShapeCommand cmd(repository_, id, 0);
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(slidePtr->getShapeCount(), 0);
}

TEST_F(CommandsTest, RemoveShapeCommand_CanBeUndone) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    int id = repository_->addSlide(std::move(slide));
    auto* slidePtr = repository_->getSlide(id);
    slidePtr->addShape(model::SlideFactory::createShape("circle", 1.0));
    UndoableRemoveShapeCommand cmd(repository_, id, 0);
    cmd.execute(output_);
    bool undone = cmd.undo();
    
    EXPECT_TRUE(undone);
    EXPECT_EQ(slidePtr->getShapeCount(), 1);
}

TEST_F(CommandsTest, SaveCommand_SavesFile) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    repository_->addSlide(std::move(slide));
    std::string filename = "test_save.json";
    SaveCommand cmd(repository_, serializer_, filename);
    bool success = cmd.execute(output_);
    EXPECT_TRUE(success);
    
    std::ifstream file(filename);
    EXPECT_TRUE(file.good());
    file.close();
    
    std::remove(filename.c_str());
}

TEST_F(CommandsTest, LoadCommand_LoadsFile) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    repository_->addSlide(std::move(slide));
    
    std::string filename = "test_load.json";
    serializer_->save(repository_.get(), filename);
    
    // Clear and load
    repository_->clear();
    LoadCommand cmd(repository_, serializer_, filename);
    
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
    EXPECT_EQ(repository_->getSlideCount(), 1);
    
    std::remove(filename.c_str());
}

TEST_F(CommandsTest, DisplayCommand_DisplaysSlides) {
    auto slide = model::SlideFactory::createSlide(0, "Title", "Content", "Theme");
    repository_->addSlide(std::move(slide));
    
    DisplayCommand cmd(repository_, view_);
    
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
}

TEST_F(CommandsTest, ExitCommand_AlwaysSucceeds) {
    ExitCommand cmd;
    
    bool success = cmd.execute(output_);
    
    EXPECT_TRUE(success);
}