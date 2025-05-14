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
SET(CLIENT_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include/client)
SET(SERVER_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include/server)
SET(SHARED_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR}/include/shared)

# Custom function to collect source files in a directory
function(collect_sources dir out_var)
    file(GLOB_RECURSE sources 
        "${dir}/*.cpp"
    )
    set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

# Add nlohmann_json
# Try to find the package first
find_package(nlohmann_json 3.12.0 QUIET)

# If not found, download and install it
if(NOT nlohmann_json_FOUND)
    message(STATUS "nlohmann_json not found, fetching it...")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
    )
    FetchContent_MakeAvailable(nlohmann_json)
    message(STATUS "nlohmann_json has been fetched and made available")
else()
    message(STATUS "Found nlohmann_json: ${nlohmann_json_DIR}")
endif()

# Add magic_enum
# Try to find the package first
find_package(magic_enum CONFIG QUIET)

# If not found, download and install it
if(NOT magic_enum_FOUND)
    message(STATUS "magic_enum not found, fetching it...")
    FetchContent_Declare(
        magic_enum
        GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
        GIT_TAG v0.9.5  # Use the latest version available
    )
    FetchContent_MakeAvailable(magic_enum)
    message(STATUS "magic_enum has been fetched and made available")
else()
    message(STATUS "Found magic_enum: ${magic_enum_DIR}")
endif()

# Dependencies
add_subdirectory(dependencies/asio)
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glew)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/stb_image)
add_subdirectory(dependencies/csv_parser)

# Client/Server build
add_subdirectory(src/shared)
add_subdirectory(src/client)
add_subdirectory(src/server)