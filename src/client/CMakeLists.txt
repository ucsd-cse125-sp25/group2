cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(barnyard_breakout LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EXISTS "${STAGED_INSTALL_PREFIX}/lib64")
    set(_LIBDIR "lib64")
elseif(EXISTS "${STAGED_INSTALL_PREFIX}/lib")
    set(_LIBDIR "lib")
else()
    message(FATAL_ERROR "No 'lib' or 'lib64' directory found in ${STAGED_INSTALL_PREFIX}")
endif()

file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/${_LIBDIR}/cmake/glfw3" glfw3_DIR)
file(TO_NATIVE_PATH "${STAGED_INSTALL_PREFIX}/${_LIBDIR}/cmake/glew" GLEW_DIR)

message(STATUS "glfw3_DIR is... ${glfw3_DIR}")
message(STATUS "GLEW_DIR is... ${GLEW_DIR}")

find_package(glfw3 REQUIRED CONFIG)

set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED CONFIG)

set(CLIENT_SRCS
    main.cpp
    client.cpp
    camera.cpp
    shader.cpp
    cube.cpp
)

add_executable(main ${CLIENT_SRCS})

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries(main PRIVATE GLEW::glew_s glfw)