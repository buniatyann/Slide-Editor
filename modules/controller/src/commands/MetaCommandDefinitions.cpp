#include "controller/commands/MetaCommandDefinitions.hpp"
#include "controller/CommandContext.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"

namespace slideEditor::controller {

namespace {
    CommandContext* getContext(void* contextPtr) {
        if (!contextPtr) {
            throw std::runtime_error("Context pointer is null");
        }
        return static_cast<CommandContext*>(contextPtr);
    }
}

// ========================================
// CreateMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createCreateMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        // Validate dependencies using interface
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        auto repo = context->getRepository();
        return std::make_unique<UndoableCreateCommand>(
            repo, 
            args[0],  // title
            args[1],  // content
            args[2]   // theme
        );
    };
    
    return std::make_unique<MetaCommand>(
        "create", 
        "Creates a new slide with the specified title, content, and theme.",
        "ACTION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"title", "identifier", "The slide title", true},
            {"content", "identifier", "The slide content", true},
            {"theme", "identifier", "The slide theme", true}
        }
    );
}

// ========================================
// AddShapeMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createAddShapeMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        auto repo = context->getRepository();
        // Parse and validate arguments
        int id = std::stoi(args[0]);
        std::string type = args[1];
        double scale = std::stod(args[2]);
        // Optional color arguments with defaults
        std::string borderColor = args.size() > 3 ? args[3] : "black";
        std::string fillColor = args.size() > 4 ? args[4] : "white";
        
        return std::make_unique<UndoableAddShapeCommand>(
            repo, id, type, scale, borderColor, fillColor
        );
    };
    
    return std::make_unique<MetaCommand>(
        "addshape", 
        "Adds a shape to the slide with the given ID. Types: Circle, Rectangle, Triangle, Ellipse",
        "ACTION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"id", "int", "The slide ID", true},
            {"type", "identifier", "Shape type (circle, rectangle, triangle, ellipse)", true},
            {"scale", "double", "Scale factor for the shape", true},
            {"borderColor", "identifier", "Border color (optional, default: black)", false},
            {"fillColor", "identifier", "Fill color (optional, default: white)", false}
        }
    );
}

// ========================================
// RemoveShapeMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createRemoveShapeMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }

        auto repo = context->getRepository();
        int id = std::stoi(args[0]);
        size_t index = std::stoull(args[1]);
        
        return std::make_unique<UndoableRemoveShapeCommand>(repo, id, index);
    };
    
    return std::make_unique<MetaCommand>(
        "removeshape", 
        "Removes the shape at the specified index from the slide.",
        "ACTION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"id", "int", "The slide ID", true},
            {"index", "int", "The shape index to remove", true}
        }
    );
}

// ========================================
// UndoMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createUndoMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args; 
        auto* context = getContext(contextPtr);
        
        // Validate dependencies using interface
        if (!context->hasHistory()) {
            throw std::runtime_error("CommandHistory not available in context");
        }
        
        if (!context->hasView()) {
            throw std::runtime_error("View not available in context");
        }
        
        auto* typedContext = static_cast<CommandContext*>(context);
        auto history = typedContext->getHistoryTyped();
        auto view = context->getView();
        
        return std::make_unique<UndoCommand>(history, view);
    };
    
    return std::make_unique<MetaCommand>(
        "undo", 
        "Undoes the last ACTION (structural change).",
        "META",
        creator
        // No arguments
    );
}

// ========================================
// RedoMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createRedoMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // No arguments
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasHistory()) {
            throw std::runtime_error("CommandHistory not available in context");
        }
        
        if (!context->hasView()) {
            throw std::runtime_error("View not available in context");
        }
        
        auto* typedContext = static_cast<CommandContext*>(context);
        auto history = typedContext->getHistoryTyped();
        auto view = context->getView();
        
        return std::make_unique<RedoCommand>(history, view);
    };
    
    return std::make_unique<MetaCommand>(
        "redo", 
        "Redoes the last undone ACTION.",
        "META",
        creator
        // No arguments
    );
}

// ========================================
// SaveMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createSaveMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        if (!context->hasSerializer()) {
            throw std::runtime_error("Serializer not available in context");
        }
        
        auto repo = context->getRepository();
        auto serializer = context->getSerializer();
        
        return std::make_unique<SaveCommand>(repo, serializer, args[0]);
    };
    
    return std::make_unique<MetaCommand>(
        "save", 
        "Saves the presentation to a JSON file.",
        "OPERATION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"filename", "identifier", "The file to save to", true}
        }
    );
}

// ========================================
// LoadMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createLoadMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        if (!context->hasSerializer()) {
            throw std::runtime_error("Serializer not available in context");
        }
        
        auto repo = context->getRepository();
        auto serializer = context->getSerializer();
        
        return std::make_unique<LoadCommand>(repo, serializer, args[0]);
    };
    
    return std::make_unique<MetaCommand>(
        "load", 
        "Loads a presentation from a JSON file.",
        "OPERATION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"filename", "identifier", "The file to load from", true}
        }
    );
}

// ========================================
// DisplayMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createDisplayMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;  // No arguments
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        if (!context->hasView()) {
            throw std::runtime_error("View not available in context");
        }
        
        auto repo = context->getRepository();
        auto view = context->getView();
        
        return std::make_unique<DisplayCommand>(repo, view);
    };
    
    return std::make_unique<MetaCommand>(
        "display", 
        "Displays all slides in the presentation with their details.",
        "QUERY",
        creator
        // No arguments
    );
}

// ========================================
// DrawMetaCommand
// ========================================

std::unique_ptr<core::IMetaCommand> createDrawMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRepository()) {
            throw std::runtime_error("Repository not available in context");
        }
        
        if (!context->hasView()) {
            throw std::runtime_error("View not available in context");
        }
        
        auto repo = context->getRepository();
        auto view = context->getView();
        // Optional filename argument
        std::string filename = args.empty() ? "presentation.svg" : args[0];
        
        return std::make_unique<DrawCommand>(repo, view, filename);
    };
    
    return std::make_unique<MetaCommand>(
        "draw", 
        "Generates an SVG file of the presentation and opens it in the browser.",
        "OPERATION",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"filename", "identifier", "Output SVG filename (optional, default: presentation.svg)", false}
        }
    );
}

// ========================================
// HelpMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createHelpMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        auto* context = getContext(contextPtr);
        
        // Validate dependencies
        if (!context->hasRegistry()) {
            throw std::runtime_error("CommandRegistry not available in context");
        }
        
        if (!context->hasView()) {
            throw std::runtime_error("View not available in context");
        }
        
        auto* typedContext = static_cast<CommandContext*>(context);
        auto* registry = typedContext->getRegistryTyped();
        auto view = context->getView();

        // Optional command argument
        std::string specificCmd = args.empty() ? "" : args[0];
        return std::make_unique<HelpCommand>(registry, view, specificCmd);
    };
    
    return std::make_unique<MetaCommand>(
        "help", 
        "Shows help for all commands or a specific command.",
        "QUERY",
        creator,
        std::initializer_list<core::ArgumentInfo>{
            {"command", "identifier", "Specific command to get help for", false}
        }
    );
}

// ========================================
// ExitMetaCommand
// ========================================
std::unique_ptr<core::IMetaCommand> createExitMetaCommand() {
    auto creator = [](const std::vector<std::string>& args, void* contextPtr) 
        -> std::unique_ptr<core::ICommand> 
    {
        std::ignore = args;       // No arguments
        std::ignore = contextPtr; // No dependencies needed
        
        return std::make_unique<ExitCommand>();
    };
    
    return std::make_unique<MetaCommand>(
        "exit", 
        "Exits the application.",
        "CONTROL",
        creator
        // No arguments
    );
}

} // namespace slideEditor::controller