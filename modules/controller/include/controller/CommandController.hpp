#ifndef COMMAND_CONTROLLER_HPP
#define COMMAND_CONTROLLER_HPP

#include "interfaces/IController.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "interfaces/IInputStream.hpp"
#include "controller/commands/CommandFactory.hpp"
#include "controller/CommandHistory.hpp"
#include <memory>
#include <string>

namespace slideEditor::controller {

class CommandController : public core::IController {
public:
    CommandController(std::shared_ptr<core::ISlideRepository> repo,
                     std::shared_ptr<core::ISerializer> serializer,
                     std::shared_ptr<core::IView> view,
                     std::shared_ptr<core::IInputStream> input);
    
    void run() override;
    bool processCommand(const std::string& commandLine) override;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::shared_ptr<core::ISerializer> serializer_;
    std::shared_ptr<core::IView> view_;
    std::shared_ptr<core::IInputStream> input_;
    
    std::shared_ptr<CommandHistory> commandHistory_;
    std::unique_ptr<CommandFactory> commandFactory_;
    
    bool running_;
    
    bool processCommandLine(const std::string& commandLine);
};

} // namespace slideEditor::controller

#endif // COMMAND_CONTROLLER_HPP