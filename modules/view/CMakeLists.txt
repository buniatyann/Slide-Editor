add_library(view
    src/cli/CliView.cpp
    src/SvgGenerator.cpp        
    src/BrowserOpener.cpp
)

target_include_directories(view PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(view PUBLIC 
    core
)

message(STATUS "Configured module: view")