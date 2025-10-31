#ifndef I_COMMAND_HPP
#define I_COMMAND_HPP

#include <string>
#include <memory>

namespace slideEditor::core {

class ISlideRepository;

class ICommand {
public:
    virtual ~ICommand() = default;
    
    virtual bool execute() = 0;
    
    virtual std::string getResultMessage() const = 0;
    
    virtual bool wasSuccessful() const = 0;
};

} // namespace slideEditor::core

#endif // I_COMMAND_HPP