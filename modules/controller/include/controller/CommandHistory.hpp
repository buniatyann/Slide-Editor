#ifndef COMMAND_HISTORY_HPP
#define COMMAND_HISTORY_HPP

#include "interfaces/IUndoableCommand.hpp"
#include <vector>
#include <memory>
#include <string>

namespace slideEditor::controller {

/**
 * CommandHistory - Manages undo/redo stacks
 * Uses two stacks: one for undo, one for redo
 */
class CommandHistory {
public:
    explicit CommandHistory(size_t maxSize = 100);
    
    void pushAction(std::unique_ptr<core::IUndoableCommand> action);
    bool undoLastAction();
    bool redoLastAction();
    
    bool canUndoAction() const;
    bool canRedoAction() const;
    size_t getUndoableActionCount() const;
    size_t getRedoableActionCount() const;
    
    std::string getLastActionToUndo() const;
    std::string getLastActionToRedo() const;
    
    void clearHistory();

private:
    std::vector<std::unique_ptr<core::IUndoableCommand>> undoStack_;
    std::vector<std::unique_ptr<core::IUndoableCommand>> redoStack_; 
    size_t maxHistorySize_;
};

} // namespace slideEditor::controller

#endif // COMMAND_HISTORY_HPP