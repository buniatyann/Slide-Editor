#include "controller/CommandRegistry.hpp"
#include <algorithm>
#include <sstream>

namespace slideEditor::controller {

void CommandRegistry::registerCommand(std::unique_ptr<core::IMetaCommand> metaCommand) {
    if (!metaCommand) {
        return;
    }
    
    std::string name = metaCommand->getName();
    std::transform(name.begin(), name.end(), name.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    commands_[name] = std::move(metaCommand);
}

const core::IMetaCommand* CommandRegistry::getMetaCommand(const std::string& name) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    auto it = commands_.find(lowerName);
    if (it != commands_.end()) {
        return it->second.get();
    }
    
    return nullptr;
}

core::IMetaCommand* CommandRegistry::getMetaCommand(const std::string& name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    
    auto it = commands_.find(lowerName);
    if (it != commands_.end()) {
        return it->second.get();
    }
    
    return nullptr;
}

bool CommandRegistry::hasCommand(const std::string& name) const {
    return getMetaCommand(name) != nullptr;
}

std::vector<std::string> CommandRegistry::getAllCommandNames() const {
    std::vector<std::string> names;
    names.reserve(commands_.size());
    for (const auto& [name, _] : commands_) {
        names.push_back(name);
    }
    
    return names;
}

std::vector<const core::IMetaCommand*> CommandRegistry::getCommandsByCategory(
    const std::string& category) const 
{
    std::vector<const core::IMetaCommand*> result;
    for (const auto& [_, metaCmd] : commands_) {
        if (metaCmd->getCategory() == category) {
            result.push_back(metaCmd.get());
        }
    }
    
    return result;
}

std::string CommandRegistry::getCommandHelp(const std::string& name) const {
    const auto* metaCmd = getMetaCommand(name);
    if (!metaCmd) {
        return "Unknown command: " + name;
    }
    
    return metaCmd->getDetailedHelp();
}

std::string CommandRegistry::getAllCommandsHelp() const {
    std::ostringstream oss;
    oss << "Available Commands:\n";
    oss << "==================\n\n";
    
    // Group by category
    std::vector<std::string> categories = {
        "ACTION", "META", "OPERATION", "QUERY", "CONTROL"
    };
    
    std::map<std::string, std::string> categoryDescriptions = {
        {"ACTION", "ACTIONS (can be undone/redone)"},
        {"META", "META-COMMANDS (control history)"},
        {"OPERATION", "OPERATIONS (not undoable)"},
        {"QUERY", "QUERIES (read-only)"},
        {"CONTROL", "CONTROL"}
    };
    
    for (const auto& category : categories) {
        auto commands = getCommandsByCategory(category);
        if (commands.empty()) {
            continue;
        }
        
        oss << categoryDescriptions[category] << ":\n";
        oss << std::string(categoryDescriptions[category].length(), '-') << "\n";
        for (const auto* cmd : commands) {
            oss << cmd->getDetailedHelp() << "\n\n";
        }
    }
    
    return oss.str();
}

} // namespace slideEditor::controller