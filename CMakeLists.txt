cmake_minimum_required(VERSION 3.31)
project(Truffle_Engine)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

find_package(glm CONFIG REQUIRED)
find_package(Vulkan)
add_subdirectory(./glfw-3.4/)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm Vulkan::Vulkan glfw)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

