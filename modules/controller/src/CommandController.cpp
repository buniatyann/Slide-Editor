#include "controller/CommandController.hpp"
#include "controller/InputHandler.hpp"
#include "controller/parser/CommandParser.hpp"
#include "controller/commands/MetaCommandDefinitions.hpp"
#include "io/InputStream.hpp"
#include "io/OutputStream.hpp"
#include <sstream>
#include <memory>
#include <iostream>

namespace slideEditor::controller {

CommandController::CommandController(std::shared_ptr<core::ISlideRepository> repo,
                                     std::shared_ptr<core::ISerializer> serializer,
                                     std::shared_ptr<core::IView> view,
                                     std::shared_ptr<core::IInputStream> input)
    : repository_(repo), serializer_(serializer), view_(view), 
      input_(input), running_(false) {
    
    commandHistory_ = std::make_shared<CommandHistory>(100);
    commandRegistry_ = std::make_unique<CommandRegistry>();
    
    context_.repository = &repository_;
    context_.serializer = &serializer_;
    context_.view = &view_;
    context_.history = &commandHistory_;
    context_.commandRegistry = commandRegistry_.get();
    
    initializeCommands();
}

void CommandController::initializeCommands() {
    commandRegistry_->registerCommand(createCreateMetaCommand());
    commandRegistry_->registerCommand(createAddShapeMetaCommand());
    commandRegistry_->registerCommand(createRemoveShapeMetaCommand());
    commandRegistry_->registerCommand(createUndoMetaCommand());
    commandRegistry_->registerCommand(createRedoMetaCommand());
    commandRegistry_->registerCommand(createSaveMetaCommand());
    commandRegistry_->registerCommand(createLoadMetaCommand());
    commandRegistry_->registerCommand(createDisplayMetaCommand());
    commandRegistry_->registerCommand(createHelpMetaCommand());
    commandRegistry_->registerCommand(createExitMetaCommand());
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
    parser.setRegistry(commandRegistry_.get());  // for generic parsing
    ParsedCommand parsed = parser.parseCommand();
    if (!parsed.isValid) {
        view_->displayError(parsed.errorMessage);
        return true;
    }
    
    const auto* metaCmd = commandRegistry_->getMetaCommand(parsed.commandName);
    if (!metaCmd) {
        view_->displayError("Unknown command: " + parsed.commandName);
        return true;
    }
    
    auto creator = metaCmd->getCreator();
    auto command = creator(parsed.arguments, &context_);
    if (!command) {
        view_->displayError("Failed to create command");
        return true;
    }
    
    auto output = std::make_shared<io::OutputStream>(std::cout);
    bool success = command->execute(*output);
    if (success) {
        if (command->isAction()) {
            auto* undoableCmd = dynamic_cast<core::IUndoableCommand*>(command.get());
            if (undoableCmd) {
                std::unique_ptr<core::IUndoableCommand> undoablePtr(
                    static_cast<core::IUndoableCommand*>(command.release())
                );

                commandHistory_->pushAction(std::move(undoablePtr));
            }
        }
    }
    
    return parsed.commandName != "exit";
}

} // namespace slideEditor::controller