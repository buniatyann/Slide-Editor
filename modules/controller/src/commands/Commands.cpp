#include "controller/commands/Commands.hpp"
#include "controller/CommandRegistry.hpp" 
#include "../../model/SlideFactory.hpp"
#include <sstream>

namespace slideEditor::controller {

SaveCommand::SaveCommand(std::shared_ptr<core::ISlideRepository> repo,
                         std::shared_ptr<core::ISerializer> serializer,
                         std::string filename)
    : repository_(repo), serializer_(serializer),
      filename_(std::move(filename)), success_(false) {}

bool SaveCommand::execute(core::IOutputStream& output) {
    if (!repository_ || !serializer_) {
        success_ = false;
        message_ = "Error: Required components not available";
        output.writeLine("[ERROR] " + message_);

        return false;
    }
    
    bool saved = serializer_->save(repository_.get(), filename_);
    if (saved) {
        success_ = true;
        message_ = "Presentation saved to '" + filename_ + "'";
        output.writeLine(message_);
        return true;
    }
    
    success_ = false;
    message_ = "Error: " + serializer_->getLastError();
    output.writeLine("[ERROR] " + message_);

    return false;
}

std::string SaveCommand::getResultMessage() const {
    return message_;
}

bool SaveCommand::wasSuccessful() const {
    return success_;
}

// ===== LoadCommand =====

LoadCommand::LoadCommand(std::shared_ptr<core::ISlideRepository> repo,
                         std::shared_ptr<core::ISerializer> serializer,
                         std::string filename)
    : repository_(repo), serializer_(serializer),
      filename_(std::move(filename)), success_(false) {}

bool LoadCommand::execute(core::IOutputStream& output) {
    if (!repository_ || !serializer_) {
        success_ = false;
        message_ = "Error: Required components not available";
        output.writeLine("[ERROR] " + message_);

        return false;
    }
    
    bool loaded = serializer_->load(repository_.get(), filename_);
    if (loaded) {
        success_ = true;
        message_ = "Presentation loaded from '" + filename_ + "'";
        output.writeLine(message_);

        return true;
    }
    
    success_ = false;
    message_ = "Error: " + serializer_->getLastError();
    output.writeLine("[ERROR] " + message_);

    return false;
}

std::string LoadCommand::getResultMessage() const {
    return message_;
}

bool LoadCommand::wasSuccessful() const {
    return success_;
}

// ===== DisplayCommand =====

DisplayCommand::DisplayCommand(std::shared_ptr<core::ISlideRepository> repo,
                               std::shared_ptr<core::IView> view)
    : repository_(repo), view_(view), success_(false) {}

bool DisplayCommand::execute(core::IOutputStream& output) {
    std::ignore = output;  // Display uses view directly
    if (!repository_ || !view_) {
        success_ = false;
        message_ = "Error: Required components not available";
        return false;
    }
    
    view_->displaySlides(repository_.get());
    success_ = true;
    message_ = "Slides displayed";

    return true;
}

std::string DisplayCommand::getResultMessage() const {
    return message_;
}

bool DisplayCommand::wasSuccessful() const {
    return success_;
}

// ===== HelpCommand =====

HelpCommand::HelpCommand(CommandRegistry* registry,
                         std::shared_ptr<core::IView> view,
                         std::string specificCommand)
    : registry_(registry), view_(view),
      specificCommand_(std::move(specificCommand)),
      success_(false) {}

bool HelpCommand::execute(core::IOutputStream& output) {
    std::ignore = output;  // Help uses view directly
    if (!registry_ || !view_) {
        success_ = false;
        message_ = "Error: Required components not available";
        return false;
    }
    
    if (specificCommand_.empty()) {
        // Show all commands
        std::string helpText = registry_->getAllCommandsHelp();
        view_->displayHelp(helpText);
        message_ = "Help displayed";
    } 
    else {
        if (registry_->hasCommand(specificCommand_)) {
            std::string helpText = registry_->getCommandHelp(specificCommand_);
            view_->displayHelp(helpText);
            message_ = "Help for '" + specificCommand_ + "' displayed";
        } 
        else {
            view_->displayError("Unknown command: " + specificCommand_);
            message_ = "Error: Unknown command";
        }
    }
    
    success_ = true;
    return true;
}

std::string HelpCommand::getResultMessage() const {
    return message_;
}

bool HelpCommand::wasSuccessful() const {
    return success_;
}

// ===== ExitCommand =====

ExitCommand::ExitCommand() : success_(false) {}

bool ExitCommand::execute(core::IOutputStream& output) {
    success_ = true;
    message_ = "Exiting...";
    output.writeLine(message_);
    
    return true;
}

std::string ExitCommand::getResultMessage() const {
    return message_;
}

bool ExitCommand::wasSuccessful() const {
    return success_;
}

} // namespace slideEditor::controller