set(TARGET_NAME client)
# Don't need this now
set(LIB_NAME client_lib)

set(CLIENT_SRCS
    main.cpp
    client.cpp
    camera.cpp
    shader.cpp
    clientnetwork.cpp 
)

add_executable(${TARGET_NAME} ${CLIENT_SRCS})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${INCLUDE_DIRECTORY}
)

# Build in standalone mode to not include Boost
target_compile_definitions(${TARGET_NAME} PRIVATE ASIO_STANDALONE)
target_compile_definitions(${TARGET_NAME} PRIVATE GLM_ENABLE_EXPERIMENTAL)

target_link_libraries(${TARGET_NAME} PRIVATE
    shared_lib
    glm
    glfw
    libglew_static 
    asio
)