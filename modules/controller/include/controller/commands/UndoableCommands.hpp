#ifndef UNDOABLE_COMMANDS_HPP
#define UNDOABLE_COMMANDS_HPP

#include "interfaces/IUndoableCommand.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/IShape.hpp"
#include <string>
#include <memory>
#include <vector>

namespace slideEditor::controller {

class UndoableCreateCommand : public core::IUndoableCommand {
public:
    UndoableCreateCommand(std::shared_ptr<core::ISlideRepository> repo,
                          std::string title,
                          std::string content,
                          std::string theme);
    
    bool execute() override;
    bool undo() override;
    bool canUndo() const override;
    std::string getDescription() const override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    std::string title_;
    std::string content_;
    std::string theme_;
    
    int createdId_;
    bool executed_;
    bool success_;
    std::string message_;
};

class UndoableAddShapeCommand : public core::IUndoableCommand {
public:
    UndoableAddShapeCommand(std::shared_ptr<core::ISlideRepository> repo,
                            int slideId,
                            std::string shapeType,
                            double scale);
    
    bool execute() override;
    bool undo() override;
    bool canUndo() const override;
    std::string getDescription() const override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    int slideId_;
    std::string shapeType_;
    double scale_;
    
    size_t addedShapeIndex_;
    bool executed_;
    bool success_;
    std::string message_;
};

class UndoableRemoveShapeCommand : public core::IUndoableCommand {
public:
    UndoableRemoveShapeCommand(std::shared_ptr<core::ISlideRepository> repo,
                               int slideId,
                               size_t shapeIndex);
    
    bool execute() override;
    bool undo() override;
    bool canUndo() const override;
    std::string getDescription() const override;
    std::string getResultMessage() const override;
    bool wasSuccessful() const override;

private:
    std::shared_ptr<core::ISlideRepository> repository_;
    int slideId_;
    size_t shapeIndex_;
    
    std::unique_ptr<core::IShape> removedShape_;  // Store for undo
    bool executed_;
    bool success_;
    std::string message_;
};

} // namespace slideEdior::controller

#endif // UNDOABLE_COMMANDS_HPP