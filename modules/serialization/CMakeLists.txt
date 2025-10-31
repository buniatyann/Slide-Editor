add_library(serialization
    src/JsonSerializer.cpp
)

target_include_directories(serialization PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(serialization PUBLIC 
    core 
    model
)