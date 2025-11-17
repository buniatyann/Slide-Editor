#ifndef META_COMMAND_HPP
#define META_COMMAND_HPP

#include "interfaces/IMetaCommand.hpp"
#include <sstream>
#include <algorithm>

namespace slideEditor::controller {

struct CommandContext {
    void* repository;
    void* serializer;
    void* view;
    void* history;
    void* commandRegistry;
};


class MetaCommand : public core::IMetaCommand {
public:
    MetaCommand(std::string name, 
                std::string description,
                std::string creator,
                std::string category)
        : name_(std::move(name)),
          description_(std::move(description)),
          creator_(std::move(creator)),
          category_(std::move(category)) {}
    
    virtual ~MetaCommand() = default;
    
    // IMetaCommand implementation
    std::string getName() const override { return name_; }
    std::string getDescription() const override { return description_; }
    std::string getCreator() const override { return creator_; }
    std::string getCategory() const override { return category_; }
    
    const std::vector<core::ArgumentInfo>& getArgumentInfo() const override {
        return arguments_;
    }
    
    size_t getRequiredArgCount() const override {
        return std::count_if(arguments_.begin(), arguments_.end(),
                             [](const core::ArgumentInfo& arg) { return arg.required; });
    }
    
    size_t getMaxArgCount() const override {
        return arguments_.size();
    }
    
    bool validateArguments(const std::vector<std::string>& args) const override {
        size_t required = getRequiredArgCount();
        size_t max = getMaxArgCount();
        
        if (args.size() < required || args.size() > max) {
            return false;
        }
        
        // Type validation (basic)
        for (size_t i = 0; i < args.size(); ++i) {
            const auto& argInfo = arguments_[i];
            if (!validateArgumentType(args[i], argInfo.type)) {
                return false;
            }
        }
        
        return true;
    }
    
    std::string getUsage() const override {
        std::ostringstream oss;
        oss << name_;
        for (const auto& arg : arguments_) {
            if (arg.required) {
                oss << " <" << arg.name << ">";
            } 
            else {
                oss << " [" << arg.name << "]";
            }
        }
        
        return oss.str();
    }
    
    std::string getDetailedHelp() const override {
        std::ostringstream oss;
        oss << getUsage() << "\n";
        oss << "  " << description_ << "\n";
        if (!arguments_.empty()) {
            oss << "\n  Arguments:\n";
            for (const auto& arg : arguments_) {
                oss << "    " << arg.name << " (" << arg.type << ")";
                if (!arg.required) oss << " [optional]";
                oss << "\n      " << arg.description << "\n";
            }
        }
        
        oss << "\n  Category: " << category_;
        oss << "\n  Creator: " << creator_;
        
        return oss.str();
    }

protected:
    std::string name_;
    std::string description_;
    std::string creator_;
    std::string category_;
    std::vector<core::ArgumentInfo> arguments_;

protected:
    // Add argument to metadata
    void addArgument(const core::ArgumentInfo& arg) {
        arguments_.push_back(arg);
    }
    
    // Type validation helper
    bool validateArgumentType(const std::string& value, const std::string& type) const {
        if (type == "string" || type == "identifier") {
            return !value.empty();
        } 
        else if (type == "int") {
            try {
                std::stoi(value);
                return true;
            } catch (...) {
                return false;
            }
        } 
        else if (type == "double") {
            try {
                std::stod(value);
                return true;
            } catch (...) {
                return false;
            }
        }
        
        return true;  // Unknown type, allow
    }
};

} // namespace slideEditor::controller

#endif // META_COMMAND_HPP