cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0012 NEW)

# Find Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "Google Test not found via find_package, searching manually...")
    
    find_path(GTEST_INCLUDE_DIR gtest/gtest.h
        PATHS
        /usr/include
        /usr/local/include
        /opt/homebrew/include
        $ENV{GTEST_ROOT}/include
    )
    
    find_library(GTEST_LIBRARY 
        NAMES gtest
        PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/aarch64-linux-gnu
        /opt/homebrew/lib
        $ENV{GTEST_ROOT}/lib
    )
    
    find_library(GTEST_MAIN_LIBRARY 
        NAMES gtest_main
        PATHS
        /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
        /usr/lib/aarch64-linux-gnu
        /opt/homebrew/lib
        $ENV{GTEST_ROOT}/lib
    )
    
    if(GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
        set(GTEST_FOUND TRUE)
        set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
        set(GTEST_LIBRARIES ${GTEST_LIBRARY})
        set(GTEST_MAIN_LIBRARIES ${GTEST_MAIN_LIBRARY})
        set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
        message(STATUS "Found Google Test manually:")
        message(STATUS "  Include: ${GTEST_INCLUDE_DIR}")
        message(STATUS "  Library: ${GTEST_LIBRARY}")
    else()
        message(WARNING "==========================================")
        message(WARNING "Google Test not found!")
        message(WARNING "==========================================")
        message(WARNING "Tests will not be built.")
        message(WARNING "")
        message(WARNING "To install Google Test:")
        message(WARNING "  Ubuntu/Debian: sudo apt-get install libgtest-dev")
        message(WARNING "  macOS: brew install googletest")
        message(WARNING "  Or build from source: https://github.com/google/googletest")
        message(WARNING "==========================================")
        return()
    endif()
endif()

# Include Google Test headers
include_directories(${GTEST_INCLUDE_DIRS})

# Helper function to create test executables
function(add_unit_test TEST_NAME SOURCE_FILE)
    set(FULL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE})
    if(NOT EXISTS ${FULL_PATH})
        message(WARNING "Test source not found: ${SOURCE_FILE} - skipping ${TEST_NAME}")
        return()
    endif()
    
    add_executable(${TEST_NAME} ${SOURCE_FILE})
    
    target_link_libraries(${TEST_NAME} PRIVATE
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
        ${GTEST_INCLUDE_DIRS}
    )
    
    # Add to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    # Set output directory
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
    )
    
    message(STATUS "  ✓ ${TEST_NAME}")
endfunction()

message(STATUS "==========================================")
message(STATUS "Configuring Tests")
message(STATUS "==========================================")

# Core Tests
message(STATUS "Core Tests:")
add_unit_test(ColorTest core/ColorTest.cpp)

# Model Tests
message(STATUS "")
message(STATUS "Model Tests:")
add_unit_test(ShapeTest model/ShapeTest.cpp)
add_unit_test(SlideTest model/SlideTest.cpp)
add_unit_test(SlideRepositoryTest model/SlideRepositoryTest.cpp)
add_unit_test(SlideFactoryTest model/SlideFactoryTest.cpp)

# Controller Tests
message(STATUS "")
message(STATUS "Controller Tests:")
add_unit_test(MetaCommandTest controller/MetaCommandTest.cpp)
add_unit_test(CommandContextTest controller/CommandContextTest.cpp)
add_unit_test(CommandHistoryTest controller/CommandHistoryTest.cpp)
add_unit_test(CommandRegistryTest controller/CommandRegistryTest.cpp)
add_unit_test(CommandsTest controller/CommandsTest.cpp)

# Parser Tests
message(STATUS "")
message(STATUS "Parser Tests:")
add_unit_test(TokenTest controller/parser/TokenTest.cpp)
add_unit_test(LexerTest controller/parser/LexerTest.cpp)
add_unit_test(CommandParserTest controller/parser/CommandParserTest.cpp)

# IO Tests
message(STATUS "")
message(STATUS "IO Tests:")
add_unit_test(InputStreamTest io/InputStreamTest.cpp)
add_unit_test(OutputStreamTest io/OutputStreamTest.cpp)

# View Tests
message(STATUS "")
message(STATUS "View Tests:")
add_unit_test(SvgGeneratorTest view/SvgGeneratorTest.cpp)
add_unit_test(CliViewTest view/CliViewTest.cpp)

# Serialization Tests
message(STATUS "")
message(STATUS "Serialization Tests:")
add_unit_test(JsonSerializerTest serialization/JsonSerializerTest.cpp)

# Integration Tests
message(STATUS "")
message(STATUS "Integration Tests:")
add_unit_test(EndToEndTest integration/EndToEndTest.cpp)

message(STATUS "==========================================")
message(STATUS "Test configuration complete")
message(STATUS "==========================================")