#include "controller/MetaCommand.hpp"
#include <sstream>
#include <algorithm>

namespace slideEditor::controller {
MetaCommand::MetaCommand(std::string name, 
                         std::string description,
                         std::string category,
                         core::CommandCreator creator,
                         std::initializer_list<core::ArgumentInfo> arguments)
    : name_(std::move(name)),
      description_(std::move(description)),
      category_(std::move(category)),
      creator_(std::move(creator)),
      arguments_(arguments) {}

std::string MetaCommand::getName() const {
    return name_;
}

std::string MetaCommand::getDescription() const {
    return description_;
}

std::string MetaCommand::getCategory() const {
    return category_;
}

core::CommandCreator MetaCommand::getCreator() const {
    return creator_;
}

const std::vector<core::ArgumentInfo>& MetaCommand::getArgumentInfo() const {
    return arguments_;
}

size_t MetaCommand::getRequiredArgCount() const {
    return std::count_if(
        arguments_.begin(), 
        arguments_.end(),
        [](const core::ArgumentInfo& arg) { 
            return arg.required; 
        }
    );
}

size_t MetaCommand::getMaxArgCount() const {
    return arguments_.size();
}

bool MetaCommand::validateArguments(const std::vector<std::string>& args) const {
    size_t required = getRequiredArgCount();
    size_t max = getMaxArgCount();
    if (args.size() < required || args.size() > max) {
        return false;
    }
    
    // Validate each argument type
    for (size_t i = 0; i < args.size(); ++i) {
        const auto& argInfo = arguments_[i];
        if (!validateArgumentType(args[i], argInfo.type)) {
            return false;
        }
    }
    
    return true;
}

std::string MetaCommand::getUsage() const {
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

std::string MetaCommand::getDetailedHelp() const {
    std::ostringstream oss;

    // Usage line
    oss << getUsage() << "\n";
    oss << "  " << description_ << "\n";
    
    // Arguments section
    if (!arguments_.empty()) {
        oss << "\n  Arguments:\n";
        for (const auto& arg : arguments_) {
            oss << "    " << arg.name << " (" << arg.type << ")";
            if (!arg.required) {
                oss << " [optional]";
            }

            oss << "\n      " << arg.description << "\n";
        }
    }
    
    oss << "\n  Category: " << category_;
    
    return oss.str();
}

bool MetaCommand::validateArgumentType(const std::string& value, 
                                       const std::string& type) const {
    if (value.empty()) {
        return false; // Empty values are invalid
    }
    
    if (type == "string" || type == "identifier") {
        return true;
    }
    
    if (type == "int") {
        try {
            std::stoi(value);
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    if (type == "double") {
        try {
            std::stod(value);
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    // Unknown type - allow by default
    return true;
}

} // namespace slideEditor::controller