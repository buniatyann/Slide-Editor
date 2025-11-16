#ifndef I_COMMAND_FACTORY_HPP
#define I_COMMAND_FACTORY_HPP

#include "ICommand.hpp"
#include <memory>
#include <vector>
#include <string>

namespace slideEditor::core {

class ISlideRepository;

class ICommandFactory {
public:
    virtual ~ICommandFactory() = default;
    virtual std::unique_ptr<ICommand> createCommand(
        const std::string& commandName,
        const std::vector<std::string>& args,
        std::shared_ptr<ISlideRepository> repository = nullptr
    ) = 0;
    
    virtual bool isValidCommand(const std::string& commandName) const = 0;
    virtual std::string getCommandHelp(const std::string& commandName) const = 0;
    virtual std::string getAllCommandsHelp() const = 0;
};

} // namespace slideEditor::core

#endif // I_COMMAND_FACTORY_HPP