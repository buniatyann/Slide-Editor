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
    
    void push(std::unique_ptr<core::IUndoableCommand> command); // Add command to the execution history
    bool undo();
    bool redo();
    
    bool canUndo() const;
    bool canRedo() const;
    
    std::string getLastUndoDescription() const;
    std::string getLastRedoDescription() const;
    
    void clear();
    
    // History info
    size_t getUndoCount() const;
    size_t getRedoCount() const;

private:
    std::vector<std::unique_ptr<core::IUndoableCommand>> undoStack_;
    std::vector<std::unique_ptr<core::IUndoableCommand>> redoStack_;    
    size_t maxHistorySize_;
};

} // namespace slideEditor::controller

#endif // COMMAND_HISTORY_HPP