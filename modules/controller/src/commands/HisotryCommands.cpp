#include "controller/commands/HistoryCommands.hpp"

namespace slideEditor::controller {

UndoCommand::UndoCommand(CommandHistory* history, core::IView* view)
    : history_(history), view_(view), success_(false) {}

bool UndoCommand::execute() {
    if (!history_) {
        success_ = false;
        message_ = "Error: Command history not available";

        return false;
    }
    
    if (!history_->canUndo()) {
        success_ = false;
        message_ = "Nothing to undo";

        return false;
    }
    
    std::string description = history_->getLastUndoDescription();
    bool undone = history_->undo();
    if (undone) {
        success_ = true;
        message_ = "Undone: " + description;
        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to undo command";

    return false;
}

std::string UndoCommand::getResultMessage() const {
    return message_;
}

bool UndoCommand::wasSuccessful() const {
    return success_;
}

RedoCommand::RedoCommand(CommandHistory* history, core::IView* view)
    : history_(history), view_(view), success_(false) {}

bool RedoCommand::execute() {
    if (!history_) {
        success_ = false;
        message_ = "Error: Command history not available";

        return false;
    }
    
    if (!history_->canRedo()) {
        success_ = false;
        message_ = "Nothing to redo";

        return false;
    }
    
    std::string description = history_->getLastRedoDescription();
    bool redone = history_->redo();
    if (redone) {
        success_ = true;
        message_ = "Redone: " + description;

        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to redo command";

    return false;
}

std::string RedoCommand::getResultMessage() const {
    return message_;
}

bool RedoCommand::wasSuccessful() const {
    return success_;
}

} // namespace slideEditor::controller