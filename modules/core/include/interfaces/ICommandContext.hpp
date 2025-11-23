#ifndef I_COMMAND_CONTEXT_HPP
#define I_COMMAND_CONTEXT_HPP
#include <memory>
#include <string>

namespace slideEditor::core {

// Forward declarations
class ISlideRepository;
class ISerializer;
class IView;

class ICommandContext {
public:
    virtual ~ICommandContext() = default;
    
    virtual bool hasRepository() const = 0;
    virtual std::shared_ptr<ISlideRepository> getRepository() const = 0;
    
    virtual bool hasSerializer() const = 0;
    virtual std::shared_ptr<ISerializer> getSerializer() const = 0;
    
    virtual bool hasView() const = 0;
    virtual std::shared_ptr<IView> getView() const = 0;
    
    virtual bool hasHistory() const = 0;
    virtual void* getHistory() const = 0;  // Returns CommandHistory* as void*
    
    virtual bool hasRegistry() const = 0;
    virtual void* getRegistry() const = 0;  // Returns CommandRegistry* as void*
    
    virtual bool isValid() const = 0;
    virtual std::string getMissingDependencies() const = 0;
};

} // namespace slideEditor::core

#endif // I_COMMAND_CONTEXT_HPP