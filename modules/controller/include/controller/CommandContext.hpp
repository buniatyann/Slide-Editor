#ifndef COMMAND_CONTEXT_HPP
#define COMMAND_CONTEXT_HPP

#include "interfaces/ICommandContext.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"
#include <memory>
#include <string>

namespace slideEditor::controller {

/**
 * This class holds all the dependencies that commands need to access.
 * It provides type-safe access to shared resources without exposing
 * raw void pointers to command implementations.
 */
class CommandContext : public core::ICommandContext {
public:
    CommandContext();
    ~CommandContext() override = default;
    
    void setRepository(std::shared_ptr<core::ISlideRepository> repository);
    void setSerializer(std::shared_ptr<core::ISerializer> serializer);
    void setView(std::shared_ptr<core::IView> view);
    void setHistory(std::shared_ptr<CommandHistory> history);
    void setRegistry(CommandRegistry* registry);
    
    
    bool hasRepository() const override;
    std::shared_ptr<core::ISlideRepository> getRepository() const override;
    
    bool hasSerializer() const override;
    std::shared_ptr<core::ISerializer> getSerializer() const override;
    
    bool hasView() const override;
    std::shared_ptr<core::IView> getView() const override;
    
    bool hasHistory() const override;
    void* getHistory() const override;
    
    bool hasRegistry() const override;
    void* getRegistry() const override;
    
    bool isValid() const override;
    std::string getMissingDependencies() const override;
    
    std::shared_ptr<CommandHistory> getHistoryTyped() const;
    CommandRegistry* getRegistryTyped() const;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::shared_ptr<core::IView> view_;
    std::shared_ptr<CommandHistory> history_;
    CommandRegistry* registry_;  // Non-owning pointer
};

} // namespace slideEditor::controller

#endif // COMMAND_CONTEXT_HPP