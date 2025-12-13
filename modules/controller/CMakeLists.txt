add_library(controller
    src/CommandController.cpp
    src/InputHandler.cpp
    src/CommandHistory.cpp
    src/CommandRegistry.cpp
    src/CommandContext.cpp
    src/MetaCommand.cpp
    src/commands/Commands.cpp
    src/commands/UndoableCommands.cpp
    src/commands/HistoryCommands.cpp
    src/commands/MetaCommandDefinitions.cpp
    src/parser/CommandParser.cpp
    src/parser/Lexer.cpp
    src/parser/LexerState.cpp
    src/parser/Token.cpp
)

target_include_directories(controller PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(controller PUBLIC 
    core 
    model
    view
    io
    serialization
)

message(STATUS "Configured module: controller")