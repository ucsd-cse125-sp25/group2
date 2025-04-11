include(ExternalProject)
ExternalProject_Add(stb_external
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW ON
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        <SOURCE_DIR>/stb_image.h 
        ${STAGED_INSTALL_PREFIX}/include/stb_image.h
    LOG_DOWNLOAD ON
)
