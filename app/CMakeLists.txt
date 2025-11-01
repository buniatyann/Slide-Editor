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

# set output directory
set_target_properties(SlideEditor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# installation rules
install(TARGETS SlideEditor 
    RUNTIME DESTINATION bin
)

message(STATUS "Configured executable: SlideEditor")