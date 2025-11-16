#include "controller/CommandHistory.hpp"

namespace slideEditor::controller {

CommandHistory::CommandHistory(size_t maxSize) 
    : maxHistorySize_(maxSize) {}

void CommandHistory::push(std::unique_ptr<core::IUndoableCommand> command) {
    if (!command) {
        return;
    }
    
    redoStack_.clear(); // if command is executed clear the redo stack
    undoStack_.push_back(std::move(command));
    if (undoStack_.size() > maxHistorySize_) {
        undoStack_.erase(undoStack_.begin());
    }
}

bool CommandHistory::undo() {
    if (!canUndo()) {
        return false;
    }
    
    auto command = std::move(undoStack_.back());
    undoStack_.pop_back();
    bool success = command->undo();
    if (success) {
        // Move to redo stack
        redoStack_.push_back(std::move(command));
    } 
    else {
        // Failed to undo, put it back
        undoStack_.push_back(std::move(command));
    }
    
    return success;
}

bool CommandHistory::redo() {
    if (!canRedo()) {
        return false;
    }
    
    auto command = std::move(redoStack_.back());
    redoStack_.pop_back();
    bool success = command->execute(); // re-execute the command;
    
    if (success) {
        undoStack_.push_back(std::move(command));
    } 
    else {
        redoStack_.push_back(std::move(command));
    }
    
    return success;
}

bool CommandHistory::canUndo() const {
    return !undoStack_.empty();
}

bool CommandHistory::canRedo() const {
    return !redoStack_.empty();
}

std::string CommandHistory::getLastUndoDescription() const {
    if (undoStack_.empty()) {
        return "";
    }
    
    return undoStack_.back()->getDescription();
}

std::string CommandHistory::getLastRedoDescription() const {
    if (redoStack_.empty()) {
        return "";
    }
    
    return redoStack_.back()->getDescription();
}

void CommandHistory::clear() {
    undoStack_.clear();
    redoStack_.clear();
}

size_t CommandHistory::getUndoCount() const {
    return undoStack_.size();
}

size_t CommandHistory::getRedoCount() const {
    return redoStack_.size();
}

} // namespace slideEditor::controller