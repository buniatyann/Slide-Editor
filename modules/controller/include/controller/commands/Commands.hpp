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
    int slideId_;
    std::string shapeType_;
    double scale_;
    
    bool success_;
    std::string message_;
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
    int slideId_;
    size_t shapeIndex_;
    
    bool success_;
    std::string message_;
};

// Save command
class SaveCommand : public core::ICommand {
public:
    SaveCommand(core::ISlideRepository* repo,
                core::ISerializer* serializer,
                std::string filename);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;
private:
    core::ISlideRepository* repository_;
    core::ISerializer* serializer_;
    std::string filename_;
    
    bool success_;
    std::string message_;
};

// Load command
class LoadCommand : public core::ICommand {
public:
    LoadCommand(core::ISlideRepository* repo,
                core::ISerializer* serializer,
                std::string filename);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ISlideRepository* repository_;
    core::ISerializer* serializer_;
    std::string filename_;
    
    bool success_;
    std::string message_;
};

// Display command
class DisplayCommand : public core::ICommand {
public:
    DisplayCommand(core::ISlideRepository* repo,
                   core::IView* view);
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ISlideRepository* repository_;
    core::IView* view_;
    
    bool success_;
    std::string message_;
};

// Help command
class HelpCommand : public core::ICommand {
public:
    HelpCommand(core::ICommandFactory* factory,
                core::IView* view,
                std::string specificCommand = "");
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    core::ICommandFactory* factory_;
    core::IView* view_;
    std::string specificCommand_;
    
    bool success_;
    std::string message_;
};

// Exit command
class ExitCommand : public core::ICommand {
public:
    ExitCommand();
    
    bool execute() override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    bool success_;
    std::string message_;
};

} // namespace slideEditor::controller

#endif // COMMANDS_HPP