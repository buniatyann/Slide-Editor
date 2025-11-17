#ifndef COMMAND_REGISTRY_HPP
#define COMMAND_REGISTRY_HPP

#include "interfaces/IMetaCommand.hpp"
#include <memory>
#include <map>
#include <string>
#include <vector>

namespace slideEditor::controller {

// Stores metadata for each command
class CommandRegistry {
public:
    CommandRegistry() = default;
    
    void registerCommand(std::unique_ptr<core::IMetaCommand> metaCommand);
    const core::IMetaCommand* getMetaCommand(const std::string& name) const;
    core::IMetaCommand* getMetaCommand(const std::string& name);
    
    // Query
    bool hasCommand(const std::string& name) const;
    std::vector<std::string> getAllCommandNames() const;
    std::vector<const core::IMetaCommand*> getCommandsByCategory(
        const std::string& category) const;
    
    // Help generation
    std::string getCommandHelp(const std::string& name) const;
    std::string getAllCommandsHelp() const;

private:
    std::map<std::string, std::unique_ptr<core::IMetaCommand>> commands_;
};

} // namespace slideEditor::controller

#endif // COMMAND_REGISTRY_HPP