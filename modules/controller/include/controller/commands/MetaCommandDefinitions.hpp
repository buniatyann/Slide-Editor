#ifndef META_COMMAND_DEFINITIONS_HPP
#define META_COMMAND_DEFINITIONS_HPP

#include "controller/MetaCommand.hpp"
#include "controller/commands/UndoableCommands.hpp"
#include "controller/commands/Commands.hpp"
#include "controller/commands/HistoryCommands.hpp"
#include "interfaces/ISlideRepository.hpp"
#include "interfaces/ISerializer.hpp"
#include "interfaces/IView.hpp"
#include "controller/CommandHistory.hpp"
#include "controller/CommandRegistry.hpp"
#include <memory>

namespace slideEditor::controller {

// Factory functions for creating MetaCommand instances
std::unique_ptr<core::IMetaCommand> createCreateMetaCommand();
std::unique_ptr<core::IMetaCommand> createAddShapeMetaCommand();
std::unique_ptr<core::IMetaCommand> createRemoveShapeMetaCommand();
std::unique_ptr<core::IMetaCommand> createUndoMetaCommand();
std::unique_ptr<core::IMetaCommand> createRedoMetaCommand();
std::unique_ptr<core::IMetaCommand> createSaveMetaCommand();
std::unique_ptr<core::IMetaCommand> createLoadMetaCommand();
std::unique_ptr<core::IMetaCommand> createDisplayMetaCommand();
std::unique_ptr<core::IMetaCommand> createHelpMetaCommand();
std::unique_ptr<core::IMetaCommand> createExitMetaCommand();

} // namespace slideEditor::controller

#endif // META_COMMAND_DEFINITIONS_HPP