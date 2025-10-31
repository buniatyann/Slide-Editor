add_library(view
    src/cli/CliView.cpp
)

target_include_directories(view PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(view PUBLIC core)