#include "controller/CommandController.hpp"
#include "controller/InputHandler.hpp"
#include "controller/parser/CommandParser.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "io/InputStream.hpp"
#include <sstream>
#include <memory>

namespace slideEditor::controller {

CommandController::CommandController(core::ISlideRepository* repo,
                                     core::ISerializer* serializer,
                                     core::IView* view,
                                     core::IInputStream* input)
    : repository_(repo), serializer_(serializer), view_(view), 
      input_(input), running_(false) {
    commandHistory_ = std::make_unique<CommandHistory>(100);
    commandFactory_ = std::make_unique<CommandFactory>(
        repo, serializer, view, commandHistory_.get()
    );
}

void CommandController::run() {
    running_ = true;
    view_->displayMessage("SlideEditor - Interactive Mode");
    view_->displayMessage("Type 'help' for available commands, 'exit' to quit.");
    view_->displayMessage("Commands that support undo/redo: create, addshape, removeshape\n");
    InputHandler inputHandler(input_);
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
    io::InputStream stream(commandLine);
    
    CommandParser parser(&stream);
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
    
    auto* undoableCmd = dynamic_cast<core::IUndoableCommand*>(command.get()); // check if it's an undoable command
    bool success = command->execute();
    if (success) {
        view_->displayMessage(command->getResultMessage());
        if (undoableCmd && 
            parsed.commandName != "undo" && 
            parsed.commandName != "redo") {
            // Transfer ownership to history
            std::unique_ptr<core::IUndoableCommand> undoablePtr(
                static_cast<core::IUndoableCommand*>(command.release())
            );

            commandHistory_->push(std::move(undoablePtr));
        }
    } 
    else {
        view_->displayError(command->getResultMessage());
    }
    
    return parsed.commandName != "exit";
}

} // namespace slideEditor::controller