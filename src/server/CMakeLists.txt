set(TARGET_NAME server)
# Don't need this now
set(LIB_NAME server_lib)

collect_sources("${CMAKE_CURRENT_SOURCE_DIR}" SERVER_SRCS)

add_executable(${TARGET_NAME} ${SERVER_SRCS})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${INCLUDE_DIRECTORY}
)

# Compile Definitions
# Build in standalone mode to not include Boost
target_compile_definitions(${TARGET_NAME} PRIVATE ASIO_STANDALONE)

target_link_libraries(${TARGET_NAME} PRIVATE
    shared_lib
    glm
    glfw
    libglew_static
    asio
)