#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include "interfaces/ICommandFactory.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace slideEditor::controller {

class CommandFactory : public core::ICommandFactory {
public:
    CommandFactory(core::ISlideRepository* repo,
                   core::ISerializer* serializer,
                   core::IView* view);
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::string& commandName,
        const std::vector<std::string>& args,
        core::ISlideRepository* repository
    ) override;
    
    bool isValidCommand(const std::string& commandName) const override;
    std::string getCommandHelp(const std::string& commandName) const override;
    std::string getAllCommandsHelp() const override;

private:
    core::ISlideRepository* repository_;
    core::ISerializer* serializer_;
    core::IView* view_;
    
    std::map<std::string, std::string> helpTexts_;
    
    void initializeHelpTexts();
};

} // namespace slideEditor::controller

#endif // COMMAND_FACTORY_HPP