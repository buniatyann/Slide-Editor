#include "controller/commands/MetaCommandDefinitions.hpp"

namespace slideEditor::controller {

std::unique_ptr<core::IMetaCommand> createCreateMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = static_cast<CommandContext*>(contextPtr);
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        auto repo = context->getRepository();
        // Arguments are already validated strings
        return std::make_unique<UndoableCreateCommand>(repo, args[0], args[1], args[2]);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "create", 
        "Creates a new slide with the specified title, content, and theme.",
        "ACTION",
        creator
    );
    
    metaCmd->addArgument({"title", "identifier", "The slide title", true});
    metaCmd->addArgument({"content", "identifier", "The slide content", true});
    metaCmd->addArgument({"theme", "identifier", "The slide theme", true});
    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createAddShapeMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
                    -> std::unique_ptr<core::ICommand> 
    {
        auto* context = static_cast<CommandContext*>(contextPtr);
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        auto repo = context->getRepository();
        int id = std::stoi(args[0]);
        double scale = std::stod(args[2]);
        
        // Optional color arguments
        std::string borderColor = args.size() > 3 ? args[3] : "black";
        std::string fillColor = args.size() > 4 ? args[4] : "white";
        
        return std::make_unique<UndoableAddShapeCommand>(
            repo, id, args[1], scale, borderColor, fillColor
        );
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "addshape", 
        "Adds a shape to the slide with the given ID. Types: Circle, Rectangle, Triangle, Ellipse",
        "ACTION",
        creator
    );
    
    metaCmd->addArgument({"id", "int", "The slide ID", true});
    metaCmd->addArgument({"type", "identifier", "Shape type (circle, rectangle, triangle, ellipse)", true});
    metaCmd->addArgument({"scale", "double", "Scale factor for the shape", true});
    metaCmd->addArgument({"borderColor", "identifier", "Border color (optional, default: black)", false});
    metaCmd->addArgument({"fillColor", "identifier", "Fill color (optional, default: white)", false});
    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createRemoveShapeMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        int id = std::stoi(args[0]);
        size_t index = std::stoull(args[1]);
        
        return std::make_unique<UndoableRemoveShapeCommand>(*repo, id, index);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "removeshape", 
        "Removes the shape at the specified index from the slide.",
        "ACTION",
        creator
    );
    
    metaCmd->addArgument({"id", "int", "The slide ID", true});
    metaCmd->addArgument({"index", "int", "The shape index to remove", true});
    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createUndoMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto history = static_cast<std::shared_ptr<CommandHistory>*>(ctx->getHistory());
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->getView());
        
        return std::make_unique<UndoCommand>(*history, *view);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "undo", 
        "Undoes the last ACTION (structural change).",
        "META",
        creator
    );
    
    // No arguments
    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createRedoMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto history = static_cast<std::shared_ptr<CommandHistory>*>(ctx->history);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->view);
        
        return std::make_unique<RedoCommand>(*history, *view);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "redo", 
        "Redoes the last undone ACTION.",
        "META",
        creator
    );
    
    // No arguments
    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createSaveMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        auto serializer = static_cast<std::shared_ptr<core::ISerializer>*>(ctx->serializer);
        
        return std::make_unique<SaveCommand>(*repo, *serializer, args[0]);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "save", 
        "Saves the presentation to a JSON file.",
        "OPERATION",
        creator
    );
    
    metaCmd->addArgument({"filename", "identifier", "The file to save to", true});    
    return metaCmd;
}


std::unique_ptr<core::IMetaCommand> createLoadMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->repository);
        auto serializer = static_cast<std::shared_ptr<core::ISerializer>*>(ctx->serializer);
        
        return std::make_unique<LoadCommand>(*repo, *serializer, args[0]);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "load", 
        "Loads a presentation from a JSON file.",
        "OPERATION",
        creator
    );
    
    metaCmd->addArgument({"filename", "identifier", "The file to load from", true});    
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createDisplayMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // Unused
        auto* ctx = static_cast<CommandContext*>(context);
        auto repo = static_cast<std::shared_ptr<core::ISlideRepository>*>(ctx->getRepository());
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->getView());
        
        return std::make_unique<DisplayCommand>(*repo, *view);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "display", 
        "Displays all slides in the presentation with their details.",
        "QUERY",
        creator
    );
    
    // No arguments
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createHelpMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* ctx = static_cast<CommandContext*>(context);
        auto registry = static_cast<CommandRegistry*>(ctx->commandRegistry);
        auto view = static_cast<std::shared_ptr<core::IView>*>(ctx->getView());        
        std::string specificCmd = args.empty() ? "" : args[0];

        return std::make_unique<HelpCommand>(registry, *view, specificCmd);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "help", 
        "Shows help for all commands or a specific command.",
        "QUERY",
        creator
    );
    
    metaCmd->addArgument({"command", "identifier", "Specific command to get help for", false});
    return metaCmd;
}

std::unique_ptr<core::IMetaCommand> createExitMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* context) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // Unused
        std::ignore = context;  // Unused
        
        return std::make_unique<ExitCommand>();
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "exit", 
        "Exits the application.",
        "CONTROL",
        creator
    );
    
    // No arguments
    return metaCmd;
}

// ========================================
// DrawMetaCommand
// ========================================

std::unique_ptr<core::IMetaCommand> createDrawMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = static_cast<CommandContext*>(contextPtr);
        
        if (!context->hasRepository() || !context->hasView()) {
            throw std::runtime_error("Repository or View not available in context");
        }
        
        auto repo = context->getRepository();
        auto view = context->getView();
        
        std::string filename = args.empty() ? "presentation.svg" : args[0];
        
        return std::make_unique<DrawCommand>(repo, view, filename);
    };
    
    auto metaCmd = std::make_unique<MetaCommand>(
        "draw", 
        "Generates an SVG file of the presentation and opens it in the browser.",
        "OPERATION",
        creator
    );
    
    metaCmd->addArgument({"filename", "identifier", "Output SVG filename (optional, default: presentation.svg)", false});
    
    return metaCmd;
}


} // namespace slideEditor::controller 