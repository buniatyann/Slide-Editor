#include <gtest/gtest.h>
#include "controller/CommandHistory.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "model/SlideRepository.hpp"
#include "io/OutputStream.hpp"
#include <memory>

using namespace slideEditor::controller;
using namespace slideEditor;

class CommandHistoryTest : public ::testing::Test {
protected:
    std::unique_ptr<CommandHistory> history_;
    std::shared_ptr<model::SlideRepository> repository_;
    
    void SetUp() override {
        history_ = std::make_unique<CommandHistory>(100);
        repository_ = std::make_shared<model::SlideRepository>();
    }
    
    std::unique_ptr<core::IUndoableCommand> createTestCommand() {
        io::OutputStream output;
        auto cmd = std::make_unique<UndoableCreateCommand>(
            repository_, "Title", "Content", "Theme"
        );
        cmd->execute(output);
        return cmd;
    }
};

TEST_F(CommandHistoryTest, Constructor_CreatesEmpty) {
    EXPECT_FALSE(history_->canUndoAction());
    EXPECT_FALSE(history_->canRedoAction());
    EXPECT_EQ(history_->getUndoableActionCount(), 0);
    EXPECT_EQ(history_->getRedoableActionCount(), 0);
}

TEST_F(CommandHistoryTest, PushAction_AddsToUndoStack) {
    auto cmd = createTestCommand();
    history_->pushAction(std::move(cmd));
    
    EXPECT_TRUE(history_->canUndoAction());
    EXPECT_FALSE(history_->canRedoAction());
    EXPECT_EQ(history_->getUndoableActionCount(), 1);
}

TEST_F(CommandHistoryTest, PushAction_ClearsRedoStack) {
    auto cmd1 = createTestCommand();
    auto cmd2 = createTestCommand();
    
    history_->pushAction(std::move(cmd1));
    history_->undoLastAction();
    
    EXPECT_TRUE(history_->canRedoAction());
    
    history_->pushAction(std::move(cmd2));
    
    EXPECT_FALSE(history_->canRedoAction());
}

TEST_F(CommandHistoryTest, UndoLastAction_MovesToRedoStack) {
    auto cmd = createTestCommand();
    history_->pushAction(std::move(cmd));
    
    bool undone = history_->undoLastAction();
    
    EXPECT_TRUE(undone);
    EXPECT_FALSE(history_->canUndoAction());
    EXPECT_TRUE(history_->canRedoAction());
}

TEST_F(CommandHistoryTest, RedoLastAction_MovesToUndoStack) {
    auto cmd = createTestCommand();
    history_->pushAction(std::move(cmd));
    history_->undoLastAction();
    
    bool redone = history_->redoLastAction();
    
    EXPECT_TRUE(redone);
    EXPECT_TRUE(history_->canUndoAction());
    EXPECT_FALSE(history_->canRedoAction());
}

TEST_F(CommandHistoryTest, UndoLastAction_ReturnsFalseWhenEmpty) {
    bool undone = history_->undoLastAction();
    
    EXPECT_FALSE(undone);
}

TEST_F(CommandHistoryTest, RedoLastAction_ReturnsFalseWhenEmpty) {
    bool redone = history_->redoLastAction();
    
    EXPECT_FALSE(redone);
}

TEST_F(CommandHistoryTest, GetLastActionToUndo_ReturnsDescription) {
    auto cmd = createTestCommand();
    history_->pushAction(std::move(cmd));
    
    std::string desc = history_->getLastActionToUndo();
    
    EXPECT_FALSE(desc.empty());
    EXPECT_NE(desc.find("Title"), std::string::npos);
}

TEST_F(CommandHistoryTest, GetLastActionToRedo_ReturnsDescription) {
    auto cmd = createTestCommand();
    history_->pushAction(std::move(cmd));
    history_->undoLastAction();
    
    std::string desc = history_->getLastActionToRedo();
    
    EXPECT_FALSE(desc.empty());
}

TEST_F(CommandHistoryTest, ClearHistory_RemovesAllActions) {
    history_->pushAction(createTestCommand());
    history_->pushAction(createTestCommand());
    history_->undoLastAction();
    
    history_->clearHistory();
    
    EXPECT_FALSE(history_->canUndoAction());
    EXPECT_FALSE(history_->canRedoAction());
    EXPECT_EQ(history_->getUndoableActionCount(), 0);
    EXPECT_EQ(history_->getRedoableActionCount(), 0);
}

TEST_F(CommandHistoryTest, MaxHistorySize_LimitsStack) {
    CommandHistory limitedHistory(2);
    
    limitedHistory.pushAction(createTestCommand());
    limitedHistory.pushAction(createTestCommand());
    limitedHistory.pushAction(createTestCommand());
    
    EXPECT_EQ(limitedHistory.getUndoableActionCount(), 2);
}