FetchContent_Declare(glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG glew-cmake-2.2.0
)

option(glew-cmake_BUILD_STATIC "Build the static glew library" ON)
option(glew-cmake_BUILD_SHARED "Do not build the shared glew library" OFF)
option(ONLY_LIBS "Do not build executables" OFF)

FetchContent_MakeAvailable(glew)