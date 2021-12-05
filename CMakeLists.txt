project(prism)

add_library(${PROJECT_NAME}
    "source/Prism.cpp"
)

target_include_directories(${PROJECT_NAME}
    INTERFACE "include"
    PRIVATE "include/prism"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC Eigen3::Eigen
)

if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE "-fPIC")
endif()
