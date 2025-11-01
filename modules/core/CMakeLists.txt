add_library(core INTERFACE)

target_include_directories(core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

message(STATUS "Configured module: core (interface library)")