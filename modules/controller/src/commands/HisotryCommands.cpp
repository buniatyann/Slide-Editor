#include "controller/commands/HistoryCommands.hpp"

namespace slideEditor::controller {

// ========================================
// UndoCommand (META-COMMAND)
// ========================================

UndoCommand::UndoCommand(std::shared_ptr<CommandHistory> history, 
                         std::shared_ptr<core::IView> view)
    : history_(history), view_(view), success_(false) {}

bool UndoCommand::execute() {
    if (!history_) {
        success_ = false;
        message_ = "Error: Command history not available";
        return false;
    }
    
    if (!history_->canUndoAction()) {
        success_ = false;
        message_ = "Nothing to undo";
        return false;
    }
    
    std::string actionDescription = history_->getLastActionToUndo();
    bool undone = history_->undoLastAction();
    if (undone) {
        success_ = true;
        message_ = "[UNDO] Undone action: " + actionDescription;

        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to undo action";

    return false;
}

std::string UndoCommand::getResultMessage() const {
    return message_;
}

bool UndoCommand::wasSuccessful() const {
    return success_;
}

// ========================================
// RedoCommand (META-COMMAND)
// ========================================


RedoCommand::RedoCommand(std::shared_ptr<CommandHistory> history, 
                         std::shared_ptr<core::IView> view)
    : history_(history), view_(view), success_(false) {}

bool RedoCommand::execute() {
    if (!history_) {
        success_ = false;
        message_ = "Error: Command history not available";
        return false;
    }
    
    if (!history_->canRedoAction()) {
        success_ = false;
        message_ = "Nothing to redo";
        return false;
    }
    
    std::string actionDescription = history_->getLastActionToRedo();
    bool redone = history_->redoLastAction();
    if (redone) {
        success_ = true;
        message_ = "[REDO] Redone action: " + actionDescription;

        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to redo action";

    return false;
}

std::string RedoCommand::getResultMessage() const {
    return message_;
}

bool RedoCommand::wasSuccessful() const {
    return success_;
}

} // namespace slideEditor::controller