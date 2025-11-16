#ifndef HISTORY_COMMANDS_HPP
#define HISTORY_COMMANDS_HPP

#include "interfaces/ICommand.hpp"
#include "controller/CommandHistory.hpp"
#include "interfaces/IView.hpp"
#include <string>

namespace slideEditor::controller {

class UndoCommand : public core::ICommand {
public:
    UndoCommand(std::shared_ptr<CommandHistory> history, 
                std::shared_ptr<core::IView> view);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::shared_ptr<CommandHistory> history_;
    std::shared_ptr<core::IView> view_;
    
    std::string message_;
    bool success_;
};

class RedoCommand : public core::ICommand {
public:
    RedoCommand(std::shared_ptr<CommandHistory> history, 
                std::shared_ptr<core::IView> view);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::shared_ptr<CommandHistory> history_;
    std::shared_ptr<core::IView> view_;
    
    bool success_;
    std::string message_;
};

} // namespace slideEditor::controller

#endif // HISTORY_COMMANDS