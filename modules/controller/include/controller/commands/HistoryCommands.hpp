#ifndef HISTORY_COMMANDS_HPP
#define HISTORY_COMMANDS_HPP

#include "interfaces/ICommand.hpp"
#include "controller/CommandHistory.hpp"
#include "interfaces/IView.hpp"
#include <string>

namespace slideEditor::controller {

class UndoCommand : public core::ICommand {
public:
    UndoCommand(CommandHistory* history, core::IView* view);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    CommandHistory* history_;
    core::IView* view_;
    
    bool success_;
    std::string message_;
};

class RedoCommand : public core::ICommand {
public:
    RedoCommand(CommandHistory* history, core::IView* view);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    CommandHistory* history_;
    core::IView* view_;
    
    bool success_;
    std::string message_;
};

} // namespace slideEditor::controller

#endif // HISTORY_COMMANDS