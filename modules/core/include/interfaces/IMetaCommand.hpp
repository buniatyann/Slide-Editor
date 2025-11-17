#pragma once
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

class IMetaCommand {
public:
    virtual ~IMetaCommand() = default;
    
    // Basic information
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getCreator() const = 0;  
    virtual std::string getCategory() const = 0;  
    
    // Argument information
    virtual const std::vector<ArgumentInfo>& getArgumentInfo() const = 0;
    virtual size_t getRequiredArgCount() const = 0;
    virtual size_t getMaxArgCount() const = 0;
    
    // Validation
    virtual bool validateArguments(const std::vector<std::string>& args) const = 0;
    
    // Command creation (factory method)
    virtual std::unique_ptr<ICommand> createCommand(
        const std::vector<std::string>& args,
        void* context  // Opaque context for dependencies
    ) const = 0;
    
    // Help generation
    virtual std::string getUsage() const = 0;
    virtual std::string getDetailedHelp() const = 0;
};

} // namespace slideEditor::core