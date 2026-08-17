cmake_minimum_required(VERSION 3.31)
project(Truffle_Engine)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(SDL EXCLUDE_FROM_ALL)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

find_package(glm CONFIG REQUIRED)
find_package(Vulkan)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm Vulkan::Vulkan SDL3::SDL3)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

