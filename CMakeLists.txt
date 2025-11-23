cmake_minimum_required(VERSION 3.15)
project(SlideEditor VERSION 1.0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror) # removed -Werror
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")

# glb include directories
include_directories(${CMAKE_SOURCE_DIR}/external)

# subdirectories in dependency order
add_subdirectory(modules/core)
add_subdirectory(modules/io)
add_subdirectory(modules/model)
add_subdirectory(modules/serialization)
add_subdirectory(modules/view)
add_subdirectory(modules/controller)
add_subdirectory(app)

# print summary
message(STATUS "==============================================")
message(STATUS "SlideEditor Configuration Summary")
message(STATUS "==============================================")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==============================================")