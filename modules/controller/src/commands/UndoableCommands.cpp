#include "controller/commands/UndoableCommands.hpp"
#include "../../model/SlideFactory.hpp"
#include <sstream>

namespace slideEditor::controller {

// ========================================
// UndoableCreateCommand (Action)
// ========================================

UndoableCreateCommand::UndoableCreateCommand(
    std::shared_ptr<core::ISlideRepository> repo,
    std::string title,
    std::string content,
    std::string theme)
    : repository_(repo), 
      title_(std::move(title)), 
      content_(std::move(content)), 
      theme_(std::move(theme)),
      createdId_(-1),
      executed_(false),
      success_(false) {}

bool UndoableCreateCommand::execute(core::IOutputStream& output) {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    auto slide = model::SlideFactory::createSlide(0, title_, content_, theme_);
    createdId_ = repository_->addSlide(std::move(slide));
    
    if (createdId_ > 0) {
        executed_ = true;
        success_ = true;
        std::ostringstream oss;
        oss << "[ACTION] Slide created successfully with ID: " << createdId_;
        message_ = oss.str();
        output.writeLine(message_);
        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to create slide";
    output.writeLine("[ERROR] " + message_);
    return false;
}

bool UndoableCreateCommand::undo() {
    if (!executed_ || createdId_ < 0 || !repository_) {
        return false;
    }
    
    bool removed = repository_->removeSlide(createdId_);
    if (removed) {
        std::ostringstream oss;
        oss << "[UNDO ACTION] Removed slide " << createdId_;
        message_ = oss.str();
        return true;
    }
    
    return false;
}

bool UndoableCreateCommand::canUndo() const {
    return executed_ && createdId_ > 0;
}

std::string UndoableCreateCommand::getDescription() const {
    std::ostringstream oss;
    oss << "Create slide '" << title_ << "'";
    return oss.str();
}

std::string UndoableCreateCommand::getResultMessage() const {
    return message_;
}

bool UndoableCreateCommand::wasSuccessful() const {
    return success_;
}

// ========================================
// UndoableAddShapeCommand (ACTION)
// ========================================

UndoableAddShapeCommand::UndoableAddShapeCommand(
    std::shared_ptr<core::ISlideRepository> repo,
    int slideId,
    std::string shapeType,
    double scale)
    : repository_(repo),
      slideId_(slideId),
      shapeType_(std::move(shapeType)),
      scale_(scale),
      addedShapeIndex_(0),
      executed_(false),
      success_(false) {}

bool UndoableAddShapeCommand::execute(core::IOutputStream& output) {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Slide with ID " << slideId_ << " not found";
        message_ = oss.str();
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    auto shape = model::SlideFactory::createShape(shapeType_, scale_);
    if (!shape) {
        success_ = false;
        message_ = "Error: Invalid shape type '" + shapeType_ + "'";
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    // Remember index for undo
    addedShapeIndex_ = slide->getShapeCount();
    
    slide->addShape(std::move(shape));
    
    executed_ = true;
    success_ = true;
    std::ostringstream oss;
    oss << "[ACTION] Shape '" << shapeType_ << "' added to slide " << slideId_;
    message_ = oss.str();
    output.writeLine(message_);
    return true;
}

bool UndoableAddShapeCommand::undo() {
    if (!executed_ || !repository_) {
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) return false;
    
    bool removed = slide->removeShape(addedShapeIndex_);
    if (removed) {
        std::ostringstream oss;
        oss << "[UNDO ACTION] Removed shape from slide " << slideId_;
        message_ = oss.str();
        return true;
    }
    
    return false;
}

bool UndoableAddShapeCommand::canUndo() const {
    return executed_;
}

std::string UndoableAddShapeCommand::getDescription() const {
    std::ostringstream oss;
    oss << "Add " << shapeType_ << " to slide " << slideId_;
    return oss.str();
}

std::string UndoableAddShapeCommand::getResultMessage() const {
    return message_;
}

bool UndoableAddShapeCommand::wasSuccessful() const {
    return success_;
}

// ========================================
// UndoableRemoveShapeCommand (ACTION)
// ========================================

UndoableRemoveShapeCommand::UndoableRemoveShapeCommand(
    std::shared_ptr<core::ISlideRepository> repo,
    int slideId,
    size_t shapeIndex)
    : repository_(repo),
      slideId_(slideId),
      shapeIndex_(shapeIndex),
      executed_(false),
      success_(false) {}

bool UndoableRemoveShapeCommand::execute(core::IOutputStream& output) {
    if (!repository_) {
        success_ = false;
        message_ = "Error: Repository not available";
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Slide with ID " << slideId_ << " not found";
        message_ = oss.str();
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    if (shapeIndex_ >= slide->getShapeCount()) {
        success_ = false;
        std::ostringstream oss;
        oss << "Error: Shape index " << shapeIndex_ << " out of range";
        message_ = oss.str();
        output.writeLine("[ERROR] " + message_);
        return false;
    }
    
    // Clone the shape before removing (for undo)
    const auto& shapes = slide->getShapes();
    removedShape_ = shapes[shapeIndex_]->clone();
    
    bool removed = slide->removeShape(shapeIndex_);
    if (removed) {
        executed_ = true;
        success_ = true;
        std::ostringstream oss;
        oss << "[ACTION] Shape at index " << shapeIndex_ 
            << " removed from slide " << slideId_;
        message_ = oss.str();
        output.writeLine(message_);
        return true;
    }
    
    success_ = false;
    message_ = "Error: Failed to remove shape";
    output.writeLine("[ERROR] " + message_);
    return false;
}

bool UndoableRemoveShapeCommand::undo() {
    if (!executed_ || !removedShape_ || !repository_) {
        return false;
    }
    
    core::ISlide* slide = repository_->getSlide(slideId_);
    if (!slide) return false;
    
    // Re-add the shape
    slide->addShape(removedShape_->clone());
    
    std::ostringstream oss;
    oss << "[UNDO ACTION] Restored shape to slide " << slideId_;
    message_ = oss.str();
    return true;
}

bool UndoableRemoveShapeCommand::canUndo() const {
    return executed_ && removedShape_ != nullptr;
}

std::string UndoableRemoveShapeCommand::getDescription() const {
    std::ostringstream oss;
    oss << "Remove shape from slide " << slideId_;
    return oss.str();
}

std::string UndoableRemoveShapeCommand::getResultMessage() const {
    return message_;
}

bool UndoableRemoveShapeCommand::wasSuccessful() const {
    return success_;
}

} // namespace slideEditor::core