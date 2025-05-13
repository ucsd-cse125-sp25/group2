set(TARGET_NAME client)
# Don't need this now
set(LIB_NAME client_lib)

collect_sources("${CMAKE_CURRENT_SOURCE_DIR}" CLIENT_SRCS)

add_executable(${TARGET_NAME} ${CLIENT_SRCS})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${CLIENT_INCLUDE_DIRECTORY} ${SHARED_INCLUDE_DIRECTORY}
)

# Compile Definitions
# Build in standalone mode to not include Boost
target_compile_definitions(${TARGET_NAME} PRIVATE ASIO_STANDALONE)
# GLM Experimental
target_compile_definitions(${TARGET_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(${TARGET_NAME} PRIVATE
    shared_lib
    glm
    glfw
    libglew_static 
    asio
    assimp
    stb_image
    csv_parser
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
)