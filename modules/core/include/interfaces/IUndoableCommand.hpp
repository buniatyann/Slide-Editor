#ifndef I_UNDOABLE_COMMAND_HPP
#define I_UNDOABLE_COMMAND_HPP

#include "ICommand.hpp"

namespace slideEditor::core {

class IUndoableCommand : public ICommand {
public:
    virtual ~IUndoableCommand() = default;
    
    virtual bool undo() = 0;
    virtual bool canUndo() const = 0;
    virtual std::string getDescription() const = 0;
};

} // namespace slideEditor::core

#endif // I_UNDOABLE_COMMAND_HPP