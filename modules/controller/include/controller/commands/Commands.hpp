#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "interfaces/ICommand.hpp"
#include "interfaces/ICommandFactory.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/IOutputStream.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "controller/CommandRegistry.hpp"
#include <string>
#include <vector>

namespace slideEditor::controller {

class CommandRegistry;

// Save command
class SaveCommand : public core::ICommand {
public:
    SaveCommand(std::shared_ptr<core::ISlideRepository> repo,
                std::shared_ptr<core::ISerializer> serializer,
                std::string filename);
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::string filename_;

    std::string message_;
    bool success_;
};

// Load command
class LoadCommand : public core::ICommand {
public:
    LoadCommand(std::shared_ptr<core::ISlideRepository> repo,
                std::shared_ptr<core::ISerializer> serializer,
                std::string filename);
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::string filename_;
    
    std::string message_;
    bool success_;
};

// Display command
class DisplayCommand : public core::ICommand {
public:
    DisplayCommand(std::shared_ptr<core::ISlideRepository> repo,
                   std::shared_ptr<core::IView> view);
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }
    
private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::IView> view_;
    
    std::string message_;
    bool success_;
};

// Help command
class HelpCommand : public core::ICommand {
public:
    HelpCommand(CommandRegistry* registry, 
                std::shared_ptr<core::IView> view,
                std::string specificCommand = "");
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    CommandRegistry* registry_;  // Changed from ICommandFactory
    std::shared_ptr<core::IView> view_;
    std::string specificCommand_;
    
    std::string message_;
    bool success_;
};

// Exit command
class ExitCommand : public core::ICommand {
public:
    ExitCommand();
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::string message_;
    bool success_;
};

class DrawCommand : public core::ICommand {
public:
    DrawCommand(std::shared_ptr<core::ISlideRepository> repo,
                std::shared_ptr<core::IView> view,
                std::string filename = "presentation.svg");
    
    bool execute(core::IOutputStream& output) override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::IView> view_;
    std::string filename_;
    
    std::string message_;
    bool success_;
};

} // namespace slideEditor::controller

#endif // COMMANDS_HPP