add_library(io
    src/InputStream.cpp
)

target_include_directories(io PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(io PUBLIC core)