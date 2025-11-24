#ifndef COMMAND_FACTORY_HPP
#define COMMAND_FACTORY_HPP

#include "interfaces/ICommandFactory.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "../CommandHistory.hpp"
// #include "CommandHistory.hpp"
#include <memory>
#include <string>
#include <vector>
#include <map>

namespace slideEditor::controller {

class CommandFactory : public core::ICommandFactory {
public:
    CommandFactory(std::shared_ptr<core::ISlideRepository> repo,
                   std::shared_ptr<core::ISerializer> serializer,
                   std::shared_ptr<core::IView> view,
                   std::shared_ptr<CommandHistory> history);
    
    std::unique_ptr<core::ICommand> createCommand(
        const std::string& commandName,
        const std::vector<std::string>& args,
        std::shared_ptr<core::ISlideRepository> repository = nullptr
    ) override;
    
    bool isValidCommand(const std::string& commandName) const override;
    std::string getCommandHelp(const std::string& commandName) const override;
    std::string getAllCommandsHelp() const override;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::shared_ptr<core::IView> view_;
    std::shared_ptr<CommandHistory> history_;
    
    std::map<std::string, std::string> helpTexts_;
    
    void initializeHelpTexts();
};

} // namespace slideEditor::controller

#endif // COMMAND_FACTORY_HPP