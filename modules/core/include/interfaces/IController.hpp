#ifndef I_CONTROLLER_HPP
#define I_CONTROLLER_HPP

#include <memory>

namespace slideEditor::core {

class IInputStream;
class IView;
class ISlideRepository;

class IController {
public:
    virtual ~IController() = default;
    // Main control loop
    virtual void run() = 0;
    virtual bool processCommand(const std::string& commandLine) = 0; // single line processing
};

} // namespace slideEditor::core

#endif // I_CONTROLLER_HPP