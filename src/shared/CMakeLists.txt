set(LIB_NAME shared_lib)

collect_sources("${CMAKE_CURRENT_SOURCE_DIR}" SHARED_SRCS)

add_library(${LIB_NAME} STATIC ${SHARED_SRCS})

target_include_directories(${LIB_NAME} PRIVATE
    ${INCLUDE_DIRECTORY}
)

target_link_libraries(${LIB_NAME} PRIVATE 
    glm
    glfw
    libglew_static
    asio
    assimp
)