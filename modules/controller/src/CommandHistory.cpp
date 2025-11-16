#include "controller/CommandHistory.hpp"

namespace slideEditor::controller {

CommandHistory::CommandHistory(size_t maxSize) 
    : maxHistorySize_(maxSize) {}

void CommandHistory::pushAction(std::unique_ptr<core::IUndoableCommand> action) {
    if (!action) {
        return;
    }
    
    redoStack_.clear(); // clear redo stack after action
    undoStack_.push_back(std::move(action));
    if (undoStack_.size() > maxHistorySize_) {
        undoStack_.erase(undoStack_.begin());
    }
}

bool CommandHistory::undoLastAction() {
    if (!canUndoAction()) {
        return false;
    }
    
    auto action = std::move(undoStack_.back());
    undoStack_.pop_back();
    bool success = action->undo();
    if (success) {
        redoStack_.push_back(std::move(action));
    } 
    else {
        undoStack_.push_back(std::move(action));
    }
    
    return success;
}

bool CommandHistory::redoLastAction() {
    if (!canRedoAction()) {
        return false;
    }
    
    auto action = std::move(redoStack_.back());
    redoStack_.pop_back();
    bool success = action->execute();
    if (success) {
        undoStack_.push_back(std::move(action));
    } 
    else {
        redoStack_.push_back(std::move(action));
    }
    
    return success;
}

bool CommandHistory::canUndoAction() const {
    return !undoStack_.empty();
}

bool CommandHistory::canRedoAction() const {
    return !redoStack_.empty();
}

std::string CommandHistory::getLastActionToUndo() const {
    if (undoStack_.empty()) {
        return "";
    }
    
    return undoStack_.back()->getDescription();
}

std::string CommandHistory::getLastActionToRedo() const {
    if (redoStack_.empty()) {
        return "";
    }
    
    return redoStack_.back()->getDescription();
}

void CommandHistory::clearHistory() {
    undoStack_.clear();
    redoStack_.clear();
}

size_t CommandHistory::getUndoableActionCount() const {
    return undoStack_.size();
}

size_t CommandHistory::getRedoableActionCount() const {
    return redoStack_.size();
}

} // namespace slideEditor::controller