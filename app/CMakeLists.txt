add_executable(SlideEditor
    src/main.cpp
)

target_link_libraries(SlideEditor PRIVATE
    core
    model
    controller
    view
    io
    serialization
)

# Set output directory
set_target_properties(SlideEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation rules
install(TARGETS SlideEditor
    RUNTIME DESTINATION bin
)

message(STATUS "Configured application: SlideEditor")