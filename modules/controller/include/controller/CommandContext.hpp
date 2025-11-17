#ifndef COMMAND_CONTEXT_HPP
#define COMMAND_CONTEXT_HPP

#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"
#include <memory>

namespace slideEditor::controller {

class CommandContext {
public:
    CommandContext();
    ~CommandContext() = default;
    
    void setRepository(std::shared_ptr<core::ISlideRepository> repository);
    void setSerializer(std::shared_ptr<core::ISerializer> serializer);
    void setView(std::shared_ptr<core::IView> view);
    void setHistory(std::shared_ptr<CommandHistory> history);
    void setRegistry(CommandRegistry* registry);
    
    std::shared_ptr<core::ISlideRepository> getRepository() const;
    std::shared_ptr<core::ISerializer> getSerializer() const;
    std::shared_ptr<core::IView> getView() const;
    std::shared_ptr<CommandHistory> getHistory() const;
    CommandRegistry* getRegistry() const;
    
    bool isValid() const;
    std::string getMissingDependencies() const;
    
    bool hasRepository() const;
    bool hasSerializer() const;
    bool hasView() const;
    bool hasHistory() const;
    bool hasRegistry() const;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::shared_ptr<core::IView> view_;
    std::shared_ptr<CommandHistory> history_;
    CommandRegistry* registry_;  // (registry owned by controller)
};

} // namespace slideEditor::controller

#endif // COMMAND_CONTEXT_HPP