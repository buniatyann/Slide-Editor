#include "controller/CommandController.hpp"
#include "controller/InputHandler.hpp"
#include "controller/parser/CommandParser.hpp"
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
    commandFactory_ = std::make_unique<CommandFactory>(repo, serializer, view);
}

void CommandController::run() {
    running_ = true;
    
    view_->displayMessage("SlideEditor - Interactive Mode");
    view_->displayMessage("Type 'help' for available commands, 'exit' to quit.\n");
    InputHandler inputHandler(input_); // input handler for input management
    while (running_ && inputHandler.hasMoreInput()) {
        view_->displayPrompt();
        // Read command line using input handler
        auto maybeCommandLine = inputHandler.readCommandLine();
        if (!maybeCommandLine.has_value()) {
            // EOF or error
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
        // Skip empty lines
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
        return true;  // Continue running despite error
    }

    auto command = commandFactory_->createCommand(
        parsed.commandName,
        parsed.arguments,
        repository_
    );
    
    if (!command) {
        view_->displayError("Invalid command or arguments");
        return true;  // Continue running
    }
    
    bool success = command->execute();    
    if (success) {
        view_->displayMessage(command->getResultMessage());
    } 
    else {
        view_->displayError(command->getResultMessage());
    }
    
    // Return false only for exit command
    return parsed.commandName != "exit";
}

} // namespace slideEditor::controller