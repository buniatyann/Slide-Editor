#ifndef META_COMMAND_HPP
#define META_COMMAND_HPP

#include "interfaces/IMetaCommand.hpp"
#include "controller/CommandContext.hpp"
#include <sstream>
#include <algorithm>
#include <initializer_list>

namespace slideEditor::controller {

class MetaCommand : public core::IMetaCommand {
public:
    MetaCommand(std::string name, 
                std::string description,
                std::string category,
                core::CommandCreator creator,
                std::initializer_list<core::ArgumentInfo> arguments = {});
    
    virtual ~MetaCommand() = default;

    std::string getName() const override;
    std::string getDescription() const override;
    std::string getCategory() const override;
    core::CommandCreator getCreator() const override;
    const std::vector<core::ArgumentInfo>& getArgumentInfo() const override;
    size_t getRequiredArgCount() const override;
    size_t getMaxArgCount() const override;
    
    bool validateArguments(const std::vector<std::string>& args) const override;
    
    std::string getDetailedHelp() const override;
    std::string getUsage() const override;

protected:
    std::string name_;
    std::string description_;
    std::string category_;
    std::vector<core::ArgumentInfo> arguments_;
    core::CommandCreator creator_;
    
    bool validateArgumentType(const std::string& value, const std::string& type) const;
};

} // namespace slideEditor::controller

#endif // META_COMMAND_HPP