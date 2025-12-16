cmake_minimum_required(VERSION 3.31)
project(Truffle_Engine)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(SDL EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm Vulkan::Vulkan SDL3::SDL3)



