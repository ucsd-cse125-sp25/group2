cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(barnyard_breakout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Make all executables to be placed directly in the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Default to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

# Required for dependencies
include(FetchContent)

# Set custom variables
SET(INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include)

# Custom function to collect source files in a directory
function(collect_sources dir out_var)
    file(GLOB_RECURSE sources 
        "${dir}/*.cpp"
    )
    set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

# Dependencies
add_subdirectory(dependencies/asio)
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glew)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/stb_image)

# Client/Server build
add_subdirectory(src/shared)
add_subdirectory(src/client)
add_subdirectory(src/server)