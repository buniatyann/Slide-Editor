add_library(model
    src/BoundingBox.cpp
    src/Color.cpp
    src/Slide.cpp
    src/SlideFactory.cpp
    src/SlideRepository.cpp
    src/shapes/Shape.cpp
)

target_include_directories(model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(model PUBLIC 
    core
)

message(STATUS "Configured module: model")