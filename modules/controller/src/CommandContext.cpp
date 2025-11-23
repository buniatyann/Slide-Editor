#include "controller/CommandContext.hpp"
#include <sstream>

namespace slideEditor::controller {

CommandContext::CommandContext() 
    : registry_(nullptr) {}

void CommandContext::setRepository(std::shared_ptr<core::ISlideRepository> repository) {
    repository_ = repository;
}

void CommandContext::setSerializer(std::shared_ptr<core::ISerializer> serializer) {
    serializer_ = serializer;
}

void CommandContext::setView(std::shared_ptr<core::IView> view) {
    view_ = view;
}

void CommandContext::setHistory(std::shared_ptr<CommandHistory> history) {
    history_ = history;
}

void CommandContext::setRegistry(CommandRegistry* registry) {
    registry_ = registry;
}

bool CommandContext::hasRepository() const {
    return repository_ != nullptr;
}

std::shared_ptr<core::ISlideRepository> CommandContext::getRepository() const {
    return repository_;
}

bool CommandContext::hasSerializer() const {
    return serializer_ != nullptr;
}

std::shared_ptr<core::ISerializer> CommandContext::getSerializer() const {
    return serializer_;
}

bool CommandContext::hasView() const {
    return view_ != nullptr;
}

std::shared_ptr<core::IView> CommandContext::getView() const {
    return view_;
}

bool CommandContext::hasHistory() const {
    return history_ != nullptr;
}

void* CommandContext::getHistory() const {
    return history_.get();
}

bool CommandContext::hasRegistry() const {
    return registry_ != nullptr;
}

void* CommandContext::getRegistry() const {
    return registry_;
}

// Validation
bool CommandContext::isValid() const {
    return repository_ != nullptr &&
           serializer_ != nullptr &&
           view_ != nullptr &&
           history_ != nullptr &&
           registry_ != nullptr;
}

std::string CommandContext::getMissingDependencies() const {
    std::ostringstream oss;
    bool first = true;
    
    if (!repository_) {
        oss << "Repository";
        first = false;
    }
    
    if (!serializer_) {
        if (!first) {
            oss << ", ";
        }

        oss << "Serializer";
        first = false;
    }
    
    if (!view_) {
        if (!first) {
            oss << ", ";
        }

        oss << "View";
        first = false;
    }
    
    if (!history_) {
        if (!first) {
            oss << ", ";
        }

        oss << "History";
        first = false;
    }
    
    if (!registry_) {
        if (!first) {
            oss << ", ";
        }

        oss << "Registry";
    }
    
    return oss.str();
}


std::shared_ptr<CommandHistory> CommandContext::getHistoryTyped() const {
    return history_;
}

CommandRegistry* CommandContext::getRegistryTyped() const {
    return registry_;
}

} // namespace slideEditor::controller