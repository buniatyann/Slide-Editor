#ifndef COMMAND_CONTROLLER_HPP
#define COMMAND_CONTROLLER_HPP

#include "interfaces/IController.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "interfaces/IInputStream.hpp"
#include "controller/commands/CommandFactory.hpp"
#include <memory>
#include <string>

namespace slideEditor::controller {

class CommandController : public core::IController {
public:
    CommandController(core::ISlideRepository* repo,
                     core::ISerializer* serializer,
                     core::IView* view,
                     core::IInputStream* input);
    
    void run() override;
    bool processCommand(const std::string& commandLine) override;

private:
    core::ISlideRepository* repository_;
    core::ISerializer* serializer_;
    core::IView* view_;
    core::IInputStream* input_;

    std::unique_ptr<CommandFactory> commandFactory_;
    bool running_;
    
    // Helper method
    bool processCommandLine(const std::string& commandLine);
};

} // namespace slideEditor::controller

#endif // COMMAND_CONTROLLER_HPP