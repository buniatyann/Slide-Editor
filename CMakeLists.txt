cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0012 NEW)
project(SlideEditor VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build unit tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# Coverage flags
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
endif()

# Print configuration
message(STATUS "==============================================")
message(STATUS "SlideEditor v${PROJECT_VERSION}")
message(STATUS "==============================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Enable coverage: ${ENABLE_COVERAGE}")
message(STATUS "==============================================")

# Global include for external libraries
include_directories(${CMAKE_SOURCE_DIR}/external)

# Add modules in dependency order
add_subdirectory(modules/core)
add_subdirectory(modules/io)
add_subdirectory(modules/model)
add_subdirectory(modules/serialization)
add_subdirectory(modules/view)
add_subdirectory(modules/controller)

# Add application
add_subdirectory(app)

# Add tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Summary
message(STATUS "")
message(STATUS "==============================================")
message(STATUS "Configuration Summary")
message(STATUS "==============================================")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==============================================")
message(STATUS "")
message(STATUS "To build:")
message(STATUS "  cmake --build .")
message(STATUS "")
message(STATUS "To run tests:")
message(STATUS "  ctest --output-on-failure")
message(STATUS "")
message(STATUS "To run application:")
message(STATUS "  ./bin/SlideEditor")
message(STATUS "")
message(STATUS "==============================================")