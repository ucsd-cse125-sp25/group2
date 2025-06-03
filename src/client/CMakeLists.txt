set(TARGET_NAME client)
# Don't need this now
set(LIB_NAME client_lib)

collect_sources("${CMAKE_CURRENT_SOURCE_DIR}" CLIENT_SRCS)

add_executable(${TARGET_NAME} ${CLIENT_SRCS})

target_include_directories(${TARGET_NAME} PRIVATE 
    ${CLIENT_INCLUDE_DIRECTORY} 
    ${SHARED_INCLUDE_DIRECTORY}
    ${CMAKE_SOURCE_DIR}/dependencies/fmod/include
)

target_link_directories(client PRIVATE ${FMOD_LIB_DIR})

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
    ${FMOD_LIBS}
)

# Set C++ standard
set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WIN32)
    add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${FMOD_LIB_DIR}/fmod.dll
            ${FMOD_LIB_DIR}/fmodstudio.dll
            $<TARGET_FILE_DIR:client>
    )
endif()