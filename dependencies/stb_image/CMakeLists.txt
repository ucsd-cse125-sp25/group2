FetchContent_Declare(stb_image
    GIT_REPOSITORY https://github.com/nothings/stb
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(stb_image)

# Create interface library
FetchContent_GetProperties(stb_image SOURCE_DIR STB_SOURCE_DIR)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${STB_SOURCE_DIR})