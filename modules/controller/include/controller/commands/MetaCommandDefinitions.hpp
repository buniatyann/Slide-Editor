#ifndef META_COMMAND_DEFINITIONS_HPP
#define META_COMMAND_DEFINITIONS_HPP

#include "controller/MetaCommand.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "controller/commands/Commands.hpp"
#include "controller/commands/HistoryCommands.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"

namespace slideEditor::controller {

class CreateMetaCommand : public MetaCommand {
public:
    CreateMetaCommand() 
        : MetaCommand("create", 
                      "Creates a new slide with the specified title, content, and theme.",
                      "SlideEditor Core Team",
                      "ACTION") 
    {
        addArgument({"title", "identifier", "The slide title", true});
        addArgument({"content", "identifier", "The slide content", true});
        addArgument({"theme", "identifier", "The slide theme", true});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        
        return std::make_unique<UndoableCreateCommand>(*repo, args[0], args[1], args[2]);
    }
};

class AddShapeMetaCommand : public MetaCommand {
public:
    AddShapeMetaCommand() 
        : MetaCommand("addshape", 
                      "Adds a shape to the slide with the given ID. Types: Circle, Rectangle, Triangle, Ellipse",
                      "SlideEditor Core Team",
                      "ACTION") 
    {
        addArgument({"id", "int", "The slide ID", true});
        addArgument({"type", "identifier", "Shape type (circle, rectangle, triangle, ellipse)", true});
        addArgument({"scale", "double", "Scale factor for the shape", true});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        int id = std::stoi(args[0]);
        double scale = std::stod(args[2]);
        
        return std::make_unique<UndoableAddShapeCommand>(*repo, id, args[1], scale);
    }
};

class RemoveShapeMetaCommand : public MetaCommand {
public:
    RemoveShapeMetaCommand() 
        : MetaCommand("removeshape", 
                      "Removes the shape at the specified index from the slide.",
                      "SlideEditor Core Team",
                      "ACTION") 
    {
        addArgument({"id", "int", "The slide ID", true});
        addArgument({"index", "int", "The shape index to remove", true});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        int id = std::stoi(args[0]);
        size_t index = std::stoull(args[1]);
        
        return std::make_unique<UndoableRemoveShapeCommand>(*repo, id, index);
    }
};

// ========================================
// UndoMetaCommand
// ========================================
class UndoMetaCommand : public MetaCommand {
public:
    UndoMetaCommand() 
        : MetaCommand("undo", 
                      "Undoes the last ACTION (structural change).",
                      "SlideEditor Core Team",
                      "META") 
    {
        // No arguments
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto history = static_cast<std::shared_ptr<CommandHistory>*>(ctx->history);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->view);
        
        return std::make_unique<UndoCommand>(*history, *view);
    }
};

class RedoMetaCommand : public MetaCommand {
public:
    RedoMetaCommand() 
        : MetaCommand("redo", 
                      "Redoes the last undone ACTION.",
                      "SlideEditor Core Team",
                      "META") 
    {
        // No arguments
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto history = static_cast<std::shared_ptr<CommandHistory>*>(ctx->history);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->view);
        
        return std::make_unique<RedoCommand>(*history, *view);
    }
};

class SaveMetaCommand : public MetaCommand {
public:
    SaveMetaCommand() 
        : MetaCommand("save", 
                      "Saves the presentation to a JSON file.",
                      "SlideEditor Core Team",
                      "OPERATION") 
    {
        addArgument({"filename", "identifier", "The file to save to", true});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        auto serializer = static_cast<std::shared_ptr<core::ISerializer>*>(ctx->serializer);
        
        return std::make_unique<SaveCommand>(*repo, *serializer, args[0]);
    }
};

class LoadMetaCommand : public MetaCommand {
public:
    LoadMetaCommand() 
        : MetaCommand("load", 
                      "Loads a presentation from a JSON file.",
                      "SlideEditor Core Team",
                      "OPERATION") 
    {
        addArgument({"filename", "identifier", "The file to load from", true});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        auto serializer = static_cast<std::shared_ptr<core::ISerializer>*>(ctx->serializer);
        
        return std::make_unique<LoadCommand>(*repo, *serializer, args[0]);
    }
};

class DisplayMetaCommand : public MetaCommand {
public:
    DisplayMetaCommand() 
        : MetaCommand("display", 
                      "Displays all slides in the presentation with their details.",
                      "SlideEditor Core Team",
                      "QUERY") 
    {
        // No arguments
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->view);
        
        return std::make_unique<DisplayCommand>(*repo, *view);
    }
};

class HelpMetaCommand : public MetaCommand {
public:
    HelpMetaCommand() 
        : MetaCommand("help", 
                      "Shows help for all commands or a specific command.",
                      "SlideEditor Core Team",
                      "QUERY") 
    {
        addArgument({"command", "identifier", "Specific command to get help for", false});
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto registry = static_cast<CommandRegistry*>(ctx->commandRegistry);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->view);
        
        std::string specificCmd = args.empty() ? "" : args[0];
        return std::make_unique<HelpCommand>(registry, *view, specificCmd);
    }
};

class ExitMetaCommand : public MetaCommand {
public:
    ExitMetaCommand() 
        : MetaCommand("exit", 
                      "Exits the application.",
                      "SlideEditor Core Team",
                      "CONTROL") 
    {
        // No arguments
    }
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context) const override 
    {
        std::ignore = args;  // Unused
        std::ignore = context;  // Unused
        
        return std::make_unique<ExitCommand>();
    }
};

} // namespace slideEditor::controller

#endif // META_COMMAND_DEFINITIONS_HPP