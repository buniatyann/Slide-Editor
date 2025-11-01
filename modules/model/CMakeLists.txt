add_library(model
    src/BoundingBox.cpp
    src/Slide.cpp
    src/SlideRepository.cpp
    src/SlideFactory.cpp
    src/shapes/Circle.cpp
    src/shapes/Rectangle.cpp
    src/shapes/Triangle.cpp
    src/shapes/Ellipse.cpp
)

target_include_directories(model PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(model PUBLIC 
    core
)

message(STATUS "Configured module: model")