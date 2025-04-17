cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(barnyard_breakout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# debug
set(CMAKE_BUILD_TYPE Debug)

# ExternalProject base
set_property(DIRECTORY PROPERTY EP_BASE ${CMAKE_BINARY_DIR}/subprojects)

set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)
set(EP_SOURCE_DIR_PREFIX ${CMAKE_BINARY_DIR}/subprojects/Source)

# dependencies
add_subdirectory(dependencies)


# determine library directory
if(EXISTS "${STAGED_INSTALL_PREFIX}/lib64")
    set(_LIBDIR "lib64")
elseif(EXISTS "${STAGED_INSTALL_PREFIX}/lib")
    set(_LIBDIR "lib")
else()
    message(FATAL_ERROR "No 'lib' or 'lib64' directory found in ${STAGED_INSTALL_PREFIX}")
endif()

# dependency paths
file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/${_LIBDIR}/cmake/glfw3" glfw3_DIR)
file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/${_LIBDIR}/cmake/glew" GLEW_DIR)

message(STATUS "glfw3_DIR is... ${glfw3_DIR}")
message(STATUS "GLEW_DIR is... ${GLEW_DIR}")

# find required packages
find_package(glfw3 REQUIRED CONFIG)
set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED CONFIG)

# collect source files recursively
function(collect_sources dir out_var)
    file(GLOB_RECURSE sources 
        "${dir}/*.cpp"
    )
    set(${out_var} ${sources} PARENT_SCOPE)
endfunction()

# client sources
collect_sources("${CMAKE_CURRENT_SOURCE_DIR}/src/client" CLIENT_SRCS)

# executable
add_executable(main ${CLIENT_SRCS})

# include directories
target_include_directories(main PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client
)

# link dependencies
target_link_libraries(main PRIVATE GLEW::glew_s glfw)

# copy executable to build dir
add_custom_command(
    TARGET main
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:main>
        ${CMAKE_BINARY_DIR}/main
    COMMENT "Copying executable to build directory root"
)