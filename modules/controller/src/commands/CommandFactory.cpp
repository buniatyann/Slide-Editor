#include "controller/commands/CommandFactory.hpp"
#include "controller/commands/Commands.hpp"
#include <algorithm>
#include <sstream>

namespace slideEditor::controller {

CommandFactory::CommandFactory(core::ISlideRepository* repo,
                               core::ISerializer* serializer,
                               core::IView* view)
    : repository_(repo), serializer_(serializer), view_(view) {
    initializeHelpTexts();
}

void CommandFactory::initializeHelpTexts() {
    helpTexts_["create"] = 
        "create <title> <content> <theme>\n"
        "  Creates a new slide with the specified title, content, and theme.\n"
        "  Example: create MyTitle MyContent DarkTheme";
    
    helpTexts_["addshape"] = 
        "addshape <id> <type> <scale>\n"
        "  Adds a shape to the slide with the given ID.\n"
        "  Types: Circle, Rectangle, Triangle, Ellipse\n"
        "  Example: addshape 1 circle 2.5";
    
    helpTexts_["removeshape"] = 
        "removeshape <id> <index>\n"
        "  Removes the shape at the specified index from the slide.\n"
        "  Example: removeshape 1 0";
    
    helpTexts_["save"] = 
        "save <filename>\n"
        "  Saves the presentation to a JSON file.\n"
        "  Example: save presentation.json";
    
    helpTexts_["load"] = 
        "load <filename>\n"
        "  Loads a presentation from a JSON file.\n"
        "  Example: load presentation.json";
    
    helpTexts_["display"] = 
        "display\n"
        "  Displays all slides in the presentation with their details.";
    
    helpTexts_["help"] = 
        "help [command]\n"
        "  Shows help for all commands or a specific command.\n"
        "  Example: help create";
    
    helpTexts_["exit"] = 
        "exit\n"
        "  Exits the application.";
}

std::unique_ptr<core::ICommand> CommandFactory::createCommand(
    const std::string& commandName,
    const std::vector<std::string>& args,
    core::ISlideRepository* repository)
{
    std::string cmdLower = commandName;
    std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    // Use injected repository if provided, otherwise use factory's default
    auto* repo = repository ? repository : repository_;
    
    if (cmdLower == "create") {
        if (args.size() != 3) {
            return nullptr;
        }

        return std::make_unique<CreateCommand>(repo, args[0], args[1], args[2]);
    }
    
    if (cmdLower == "addshape") {
        if (args.size() != 3) {
            return nullptr;
        }

        try {
            int id = std::stoi(args[0]);
            double scale = std::stod(args[2]);
        
            return std::make_unique<AddShapeCommand>(repo, id, args[1], scale);
        } catch (...) {
            return nullptr;
        }
    }
    
    if (cmdLower == "removeshape") {
        if (args.size() != 2) { 
            return nullptr;
        }
        
        try {
            int id = std::stoi(args[0]);
            size_t index = std::stoull(args[1]);
        
            return std::make_unique<RemoveShapeCommand>(repo, id, index);
        } catch (...) {
            return nullptr;
        }
    }
    
    if (cmdLower == "save") {
        if (args.size() != 1) {
            return nullptr;
        }
        
        return std::make_unique<SaveCommand>(repo, serializer_, args[0]);
    }
    
    if (cmdLower == "load") {
        if (args.size() != 1) {
            return nullptr;
        }
        
        return std::make_unique<LoadCommand>(repo, serializer_, args[0]);
    }
    
    if (cmdLower == "display") {
        if (args.size() != 0) {
            return nullptr;
        }
        
        return std::make_unique<DisplayCommand>(repo, view_);
    }
    
    if (cmdLower == "help") {
        std::string specificCmd = args.empty() ? "" : args[0];
        return std::make_unique<HelpCommand>(this, view_, specificCmd);
    }
    
    if (cmdLower == "exit") {
        if (args.size() != 0) {
            return nullptr;
        }
        
        return std::make_unique<ExitCommand>();
    }
    
    return nullptr;
}

bool CommandFactory::isValidCommand(const std::string& commandName) const {
    std::string cmdLower = commandName;
    std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    return helpTexts_.find(cmdLower) != helpTexts_.end();
}

std::string CommandFactory::getCommandHelp(const std::string& commandName) const {
    std::string cmdLower = commandName;
    std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    
    auto it = helpTexts_.find(cmdLower);
    if (it != helpTexts_.end()) {
        return it->second;
    }

    return "Unknown command: " + commandName;
}

std::string CommandFactory::getAllCommandsHelp() const {
    std::ostringstream oss;
    oss << "Available Commands:\n";
    oss << "==================\n\n";
    
    for (const auto& [cmd, help] : helpTexts_) {
        oss << help << "\n\n";
    }
    
    return oss.str();
}

} // namespace slideEditor::controller