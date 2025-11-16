#include "controller/CommandController.hpp"
#include "controller/InputHandler.hpp"
#include "controller/parser/CommandParser.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "io/InputStream.hpp"
#include <sstream>
#include <memory>

namespace slideEditor::controller {

CommandController::CommandController(std::shared_ptr<core::ISlideRepository> repo,
                                     std::shared_ptr<core::ISerializer> serializer,
                                     std::shared_ptr<core::IView> view,
                                     std::shared_ptr<core::IInputStream> input)
    : repository_(repo), serializer_(serializer), view_(view), 
      input_(input), running_(false) {
    commandHistory_ = std::make_shared<CommandHistory>(100);
    commandFactory_ = std::make_unique<CommandFactory>(
        repo, serializer, view, commandHistory_
    );
}

void CommandController::run() {
    running_ = true;
    view_->displayMessage("SlideEditor - Interactive Mode");
    view_->displayMessage("Type 'help' for available commands, 'exit' to quit.");
    view_->displayMessage("═══════════════════════════════════════════════════");
    view_->displayMessage("ACTIONS (can be undone): create, addshape, removeshape");
    view_->displayMessage("META-COMMANDS: undo, redo");
    view_->displayMessage("═══════════════════════════════════════════════════\n");
    InputHandler inputHandler(input_.get());
    while (running_ && inputHandler.hasMoreInput()) {
        view_->displayPrompt();
        auto maybeCommandLine = inputHandler.readCommandLine();
        if (!maybeCommandLine.has_value()) {
            if (inputHandler.isEOF()) {
                break;
            }
            if (inputHandler.hasError()) {
                view_->displayError(inputHandler.getErrorMessage());
                break;
            }

            continue;
        }
        
        std::string commandLine = maybeCommandLine.value();
        if (commandLine.empty()) {
            continue;
        }
        
        bool continueRunning = processCommandLine(commandLine);
        if (!continueRunning) {
            running_ = false;
        }
    }
    
    view_->displayMessage("\nGoodbye!");
}

bool CommandController::processCommand(const std::string& commandLine) {
    return processCommandLine(commandLine);
}

bool CommandController::processCommandLine(const std::string& commandLine) {
    auto stream = std::make_shared<io::InputStream>(commandLine);
    CommandParser parser(stream.get());
    ParsedCommand parsed = parser.parseCommand();
    if (!parsed.isValid) {
        view_->displayError(parsed.errorMessage);
        return true;
    }
    
    auto command = commandFactory_->createCommand(
        parsed.commandName,
        parsed.arguments,
        repository_
    );
    
    if (!command) {
        view_->displayError("Invalid command or arguments");
        return true;
    }
    
    bool success = command->execute(); // execute the command
    if (success) {
        view_->displayMessage(command->getResultMessage());

        // If it's an ACTION (not undo/redo meta-command), add to history
        if (command->isAction()) {
            auto* undoableCmd = dynamic_cast<core::IUndoableCommand*>(command.get());
            if (undoableCmd) {
                // Transfer ownership to history
                std::unique_ptr<core::IUndoableCommand> undoablePtr(
                    static_cast<core::IUndoableCommand*>(command.release())
                );
    
                commandHistory_->pushAction(std::move(undoablePtr));
            }
        }
    } 
    else {
        view_->displayError(command->getResultMessage());
    }
    
    return parsed.commandName != "exit";
}

} // namespace slideEditor::controller