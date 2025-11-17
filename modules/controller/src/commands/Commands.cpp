#include "controller/commands/Commands.hpp"
#include "controller/CommandRegistry.hpp" 
#include "../../model/SlideFactory.hpp"
#include <sstream>

namespace slideEditor::controller {

// ===== CreateCommand =====

CreateCommand::CreateCommand(core::ISlideRepository* repo,
                             std::string title,
                             std::string content,
                             std::string theme)
    : repository_(repo), title_(std::move(title)), 
      content_(std::move(content)), theme_(std::move(theme)),
      success_(false), createdId_(-1) {}

bool CreateCommand::execute() {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
        return false;
    }
    
    auto slide = model::SlideFactory::createSlide(
        0,  // ID will be assigned by repository
        title_,
        content_,
        theme_
    );
    
    createdId_ = repository_->addSlide(std::move(slide));    
    if (createdId_ > 0) {
        success_ = true;
        std::ostringstream oss;
        oss << "Slide created successfully with ID: " << createdId_;
        message_ = oss.str();

        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to create slide";

    return false;
}

std::string CreateCommand::getResultMessage() const {
    return message_;
}

bool CreateCommand::wasSuccessful() const {
    return success_;
}

// ===== AddShapeCommand =====

AddShapeCommand::AddShapeCommand(core::ISlideRepository* repo,
                                 int slideId,
                                 std::string shapeType,
                                 double scale)
    : repository_(repo), slideId_(slideId), 
      shapeType_(std::move(shapeType)), scale_(scale),
      success_(false) {}

bool AddShapeCommand::execute() {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
    
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Slide with ID " << slideId_ << " not found";
        message_ = oss.str();
    
        return false;
    }
    
    auto shape = model::SlideFactory::createShape(shapeType_, scale_);
    if (!shape) {
        success_ = false;
        message_ = "Error: Invalid shape type '" + shapeType_ + "'";
    
        return false;
    }
    
    slide->addShape(std::move(shape));
    success_ = true;
    std::ostringstream oss;
    oss << "Shape '" << shapeType_ << "' added to slide " << slideId_;
    message_ = oss.str();

    return true;
}

std::string AddShapeCommand::getResultMessage() const {
    return message_;
}

bool AddShapeCommand::wasSuccessful() const {
    return success_;
}

// ===== RemoveShapeCommand =====

RemoveShapeCommand::RemoveShapeCommand(core::ISlideRepository* repo,
                                       int slideId,
                                       size_t shapeIndex)
    : repository_(repo), slideId_(slideId), 
      shapeIndex_(shapeIndex), success_(false) {}

bool RemoveShapeCommand::execute() {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
    
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Slide with ID " << slideId_ << " not found";
        message_ = oss.str();
    
        return false;
    }
    
    if (shapeIndex_ >= slide->getShapeCount()) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Shape index " << shapeIndex_ << " out of range";
        message_ = oss.str();
    
        return false;
    }
    
    bool removed = slide->removeShape(shapeIndex_);
    if (removed) {
        success_ = true;
        std::ostringstream oss;
        oss << "Shape at index " << shapeIndex_ 
            << " removed from slide " << slideId_;
        message_ = oss.str();
    
        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to remove shape";
    
    return false;
}

std::string RemoveShapeCommand::getResultMessage() const {
    return message_;
}

bool RemoveShapeCommand::wasSuccessful() const {
    return success_;
}

// ===== SaveCommand =====

SaveCommand::SaveCommand(std::shared_ptr<core::ISlideRepository> repo,
                         std::shared_ptr<core::ISerializer> serializer,
                         std::string filename)
    : repository_(repo), serializer_(serializer),
      filename_(std::move(filename)), success_(false) {}

bool SaveCommand::execute() {
    if (!repository_ || !serializer_) {
        success_ = false;
        message_ = "Error: Required components not available";

        return false;
    }
    
    bool saved = serializer_->save(repository_.get(), filename_);
    if (saved) {
        success_ = true;
        message_ = "Presentation saved to '" + filename_ + "'";
        return true;
    }
    
    success_ = false;
    message_ = "Error: " + serializer_->getLastError();

    return false;
}

std::string SaveCommand::getResultMessage() const {
    return message_;
}

bool SaveCommand::wasSuccessful() const {
    return success_;
}

// ===== LoadCommand =====

LoadCommand::LoadCommand(std::shared_ptr<core::ISlideRepository> repo,
                         std::shared_ptr<core::ISerializer> serializer,
                         std::string filename)
    : repository_(repo), serializer_(serializer),
      filename_(std::move(filename)), success_(false) {}

bool LoadCommand::execute() {
    if (!repository_ || !serializer_) {
        success_ = false;
        message_ = "Error: Required components not available";

        return false;
    }
    
    bool loaded = serializer_->load(repository_.get(), filename_);
    if (loaded) {
        success_ = true;
        message_ = "Presentation loaded from '" + filename_ + "'";
        return true;
    }
    
    success_ = false;
    message_ = "Error: " + serializer_->getLastError();

    return false;
}

std::string LoadCommand::getResultMessage() const {
    return message_;
}

bool LoadCommand::wasSuccessful() const {
    return success_;
}

// ===== DisplayCommand =====

DisplayCommand::DisplayCommand(std::shared_ptr<core::ISlideRepository> repo,
                               std::shared_ptr<core::IView> view)
    : repository_(repo), view_(view), success_(false) {}

bool DisplayCommand::execute() {
    if (!repository_ || !view_) {
        success_ = false;
        message_ = "Error: Required components not available";

        return false;
    }
    
    view_->displaySlides(repository_.get());
    success_ = true;
    message_ = "Slides displayed";

    return true;
}

std::string DisplayCommand::getResultMessage() const {
    return message_;
}

bool DisplayCommand::wasSuccessful() const {
    return success_;
}

// ===== HelpCommand =====

HelpCommand::HelpCommand(CommandRegistry* registry,
                         std::shared_ptr<core::IView> view,
                         std::string specificCommand)
    : registry_(registry), view_(view),
      specificCommand_(std::move(specificCommand)),
      success_(false) {}

bool HelpCommand::execute() {
    if (!registry_ || !view_) {
        success_ = false;
        message_ = "Error: Required components not available";

        return false;
    }
    
    if (specificCommand_.empty()) {
        std::string helpText = registry_->getAllCommandsHelp();
        view_->displayHelp(helpText);
        message_ = "Help displayed";
    } 
    else {
        if (registry_->hasCommand(specificCommand_)) {
            std::string helpText = registry_->getCommandHelp(specificCommand_);
            view_->displayHelp(helpText);
            message_ = "Help for '" + specificCommand_ + "' displayed";
        } 
        else {
            view_->displayError("Unknown command: " + specificCommand_);
            message_ = "Error: Unknown command";
        }
    }
    
    success_ = true;
    return true;
}

std::string HelpCommand::getResultMessage() const {
    return message_;
}

bool HelpCommand::wasSuccessful() const {
    return success_;
}

// ===== ExitCommand =====

ExitCommand::ExitCommand() : success_(false) {}

bool ExitCommand::execute() {
    success_ = true;
    message_ = "Exiting...";
    
    return true;
}

std::string ExitCommand::getResultMessage() const {
    return message_;
}

bool ExitCommand::wasSuccessful() const {
    return success_;
}

} // namespace slideEditor::controller