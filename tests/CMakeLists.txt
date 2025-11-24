find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Helper function to create tests
function(add_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} 
        ${GTEST_BOTH_LIBRARIES}
        pthread
        core
        model
        controller
        view
        io
        serialization
    )

    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/core/include
        ${CMAKE_SOURCE_DIR}/modules/model/include
        ${CMAKE_SOURCE_DIR}/modules/controller/include
        ${CMAKE_SOURCE_DIR}/modules/view/include
        ${CMAKE_SOURCE_DIR}/modules/io/include
        ${CMAKE_SOURCE_DIR}/modules/serialization/include
        ${CMAKE_SOURCE_DIR}/external
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
endfunction()

# Core tests
add_unit_test(ColorTest core/ColorTest.cpp)

# Model tests
add_unit_test(ShapeTest model/ShapeTest.cpp)
add_unit_test(SlideTest model/SlideTest.cpp)
add_unit_test(SlideRepositoryTest model/SlideRepositoryTest.cpp)
add_unit_test(SlideFactoryTest model/SlideFactoryTest.cpp)

# Controller tests
add_unit_test(MetaCommandTest controller/MetaCommandTest.cpp)
add_unit_test(CommandContextTest controller/CommandContextTest.cpp)
add_unit_test(CommandHistoryTest controller/CommandHistoryTest.cpp)
add_unit_test(CommandRegistryTest controller/CommandRegistryTest.cpp)
add_unit_test(CommandsTest controller/CommandsTest.cpp)

# Parser tests
add_unit_test(TokenTest controller/parser/TokenTest.cpp)
add_unit_test(LexerTest controller/parser/LexerTest.cpp)
add_unit_test(CommandParserTest controller/parser/CommandParserTest.cpp)

# IO tests
add_unit_test(InputStreamTest io/InputStreamTest.cpp)
add_unit_test(OutputStreamTest io/OutputStreamTest.cpp)

# View tests
add_unit_test(SvgGeneratorTest view/SvgGeneratorTest.cpp)
add_unit_test(CliViewTest view/CLIViewTest.cpp)

# Serialization tests
add_unit_test(JsonSerializerTest serialization/JsonSerializerTest.cpp)

# Integration tests
add_unit_test(EndToEndTest integration/EndToEndTest.cpp)

message(STATUS "Configured unit tests")