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

# Add ASan support
option(USE_SANITIZER "Build with Address Sanitizer" OFF)
# Add to your existing CMakeLists.txt, in the USE_SANITIZER section
if(USE_SANITIZER)
    if(APPLE)
        # macOS specific settings with better debug support
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g3 -O0")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g3 -O0")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
        
        # Ensure DWARF format for Apple Silicon
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-4")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gdwarf-4")
    else()
        # Similar updates for other platforms
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g3 -O0")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g3 -O0")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    endif()
    
    # Force debug build when using sanitizers
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Building with Address Sanitizer in Debug mode")
endif()

if (APPLE)
    set(FMOD_PLATFORM "mac")
    set(FMOD_LIBS fmod fmodstudio)
elseif (WIN32)
    set(FMOD_PLATFORM "windows")
    set(FMOD_LIBS fmod_vc fmodstudio_vc)
elseif (UNIX)
    set(FMOD_PLATFORM "linux")
    set(FMOD_LIBS fmod fmodstudio)
endif()

set(FMOD_ROOT ${CMAKE_SOURCE_DIR}/dependencies/fmod)
set(FMOD_LIB_DIR ${FMOD_ROOT}/${FMOD_PLATFORM}/lib)

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

# Dependencies
add_subdirectory(dependencies/asio)
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glew)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/stb_image)
add_subdirectory(dependencies/nlohmann_json)
add_subdirectory(dependencies/magic_enum)

# Client/Server build
add_subdirectory(src/shared)
add_subdirectory(src/client)
add_subdirectory(src/server)