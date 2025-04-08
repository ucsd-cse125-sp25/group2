cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(glfw_opengl_cmake_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# debug
set(CMAKE_BUILD_TYPE Debug)

# ExternalProject base
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
set(EP_SOURCE_DIR_PREFIX ${CMAKE_BINARY_DIR}/subprojects/Source)

add_subdirectory(dependencies)

add_subdirectory(src/client)

include(ExternalProject)

ExternalProject_Add(${PROJECT_NAME}_client_core
  DEPENDS GLFW_external GLEW_external glm_external
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/client
  CMAKE_ARGS
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DSTAGED_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
  CMAKE_CACHE_ARGS
  -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
  BUILD_ALWAYS ON
  INSTALL_COMMAND ""
)

ExternalProject_Add_Step(${PROJECT_NAME}_client_core copy_executable
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/subprojects/Build/${PROJECT_NAME}_client_core/main
    ${CMAKE_BINARY_DIR}/main
  DEPENDEES build
  ALWAYS 1
)