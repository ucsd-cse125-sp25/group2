FetchContent_Declare(glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0
)

set(glew-cmake_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glew)