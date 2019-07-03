# project(prism)

set(CMAKE_CXX_STANDARD 14)

add_library(prism
    "include/prism/Prism.hpp"
    "source/Prism.cpp"
)

target_include_directories(prism
    INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/prism"
)

target_link_libraries(prism
    PUBLIC Eigen3::Eigen
)
