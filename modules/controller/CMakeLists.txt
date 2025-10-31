add_library(controller
    src/CommandController.cpp
    src/InputHandler.cpp          # ADDED
    src/commands/Commands.cpp
    src/commands/CommandFactory.cpp
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
    io
)