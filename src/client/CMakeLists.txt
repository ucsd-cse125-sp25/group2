set(TARGET_NAME client)
# Don't need this now
set(LIB_NAME client_lib)

collect_sources("${CMAKE_CURRENT_SOURCE_DIR}" CLIENT_SRCS)

add_executable(${TARGET_NAME} ${CLIENT_SRCS})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${CLIENT_INCLUDE_DIRECTORY} ${SHARED_INCLUDE_DIRECTORY}
)

# Platform-specific configurations
if(APPLE)
    # Mac-specific settings
    target_compile_definitions(${TARGET_NAME} PRIVATE 
        GL_SILENCE_DEPRECATION
        ASIO_STANDALONE
    )
    
    # Link Mac frameworks
    target_link_libraries(${TARGET_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa" 
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_compile_definitions(${TARGET_NAME} PRIVATE ASIO_STANDALONE)
endif()

target_link_libraries(${TARGET_NAME} PRIVATE
    shared_lib
    glfw
    libglew_static
    glm
    asio
    assimp
    stb_image
    nlohmann_json::nlohmann_json
    magic_enum::magic_enum
)

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)