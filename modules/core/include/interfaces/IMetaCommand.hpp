#ifndef I_META_COMMAND_HPP
#define I_META_COMMAND_HPP

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace slideEditor::core {

class ICommand;

struct ArgumentInfo {
    std::string name;           // Argument name (e.g., "title", "id", "scale")
    std::string type;           // Type: "string", "int", "double", "identifier"
    std::string description;    // Human-readable description
    bool required;              // Is this argument required?
    
    ArgumentInfo(std::string n, std::string t, std::string desc, bool req = true)
        : name(std::move(n)), type(std::move(t)), 
          description(std::move(desc)), required(req) {}
};

using CommandCreator = std::function<std::unique_ptr<ICommand>(
    const std::vector<std::string>& args,
    void* context
)>;

class IMetaCommand {
public:
    virtual ~IMetaCommand() = default;
    
    // Basic information
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getCategory() const = 0;  // "ACTION", "META", "OPERATION", "QUERY", "CONTROL"
    
    virtual CommandCreator getCreator() const = 0;
    
    // Argument information
    virtual const std::vector<ArgumentInfo>& getArgumentInfo() const = 0;
    virtual size_t getRequiredArgCount() const = 0;
    virtual size_t getMaxArgCount() const = 0;
    
    virtual bool validateArguments(const std::vector<std::string>& args) const = 0;
    
    // Help generation
    virtual std::string getUsage() const = 0;
    virtual std::string getDetailedHelp() const = 0;
};

} // namespace slideEditor::core

#endif // I_META_COMMAND_HPP