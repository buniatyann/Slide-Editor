#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "interfaces/ICommand.hpp"
#include "interfaces/ICommandFactory.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include <string>
#include <vector>

namespace slideEditor::controller {

// Create slide command
class CreateCommand : public core::ICommand {
public:
    CreateCommand(core::ISlideRepository* repo,
                  std::string title,
                  std::string content,
                  std::string theme);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ISlideRepository* repository_;
    std::string title_;
    std::string content_;
    std::string theme_;    
    bool success_;
    std::string message_;
    int createdId_;
};

// Add shape command
class AddShapeCommand : public core::ICommand {
public:
    AddShapeCommand(core::ISlideRepository* repo,
                    int slideId,
                    std::string shapeType,
                    double scale);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ISlideRepository* repository_;
    std::string shapeType_;
    double scale_;
    int slideId_;
    
    std::string message_;
    bool success_;
};

// Remove shape command
class RemoveShapeCommand : public core::ICommand {
public:
    RemoveShapeCommand(core::ISlideRepository* repo,
                       int slideId,
                       size_t shapeIndex);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ISlideRepository* repository_;
    size_t shapeIndex_;
    int slideId_;
    
    std::string message_;
    bool success_;
};

// Save command
class SaveCommand : public core::ICommand {
public:
    SaveCommand(std::shared_ptr<core::ISlideRepository> repo,
                std::shared_ptr<core::ISerializer> serializer,
                std::string filename);
    
    bool execute() override;
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
    
    bool execute() override;
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
    
    bool execute() override;
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
    HelpCommand(core::ICommandFactory* factory,
                std::shared_ptr<core::IView> view,
                std::string specificCommand = "");
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    core::ICommandFactory* factory_;  // Can it stay as raw pointer (non-owning) ?
    std::shared_ptr<core::IView> view_;
    std::string specificCommand_;
    
    std::string message_;
    bool success_;
};

// Exit command
class ExitCommand : public core::ICommand {
public:
    ExitCommand();
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
    bool isAction() const override { return false; }

private:
    std::string message_;
    bool success_;
};

} // namespace slideEditor::controller

#endif // COMMANDS_HPP