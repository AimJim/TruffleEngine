cmake_minimum_required(VERSION 3.31)
project(Truffle_Engine)

add_executable(${PROJECT_NAME}
    src/main.cpp
)



# function (add_slang_shader_target TARGET)
#   cmake_parse_arguments ("SHADER" "" "" "SOURCES" ${ARGN})
#   set (SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/src/shaders)
#   set (ENTRY_POINTS -entry vertMain -entry fragMain)
#   add_custom_command (
#           OUTPUT ${SHADERS_DIR}
#           COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}
#   )
#   add_custom_command (
#           OUTPUT  ${SHADERS_DIR}/slang.spv
#           COMMAND ${SLANGC_EXECUTABLE} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o slang.spv
#           WORKING_DIRECTORY ${SHADERS_DIR}
#           DEPENDS ${SHADERS_DIR} ${SHADER_SOURCES}
#           COMMENT "Compiling Slang Shaders"
#           VERBATIM
#   )
#   add_custom_target (${TARGET} DEPENDS ${SHADERS_DIR}/slang.spv)
# endfunction()

# add_slang_shader_target(shader SOURCES ${SHADER_SLANG_SOURCES})
# add_dependencies(${PROJECT_NAME} shader)

find_package(glm CONFIG REQUIRED)
find_package(Vulkan)
add_subdirectory(./glfw/)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm Vulkan::Vulkan glfw)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

