FetchContent_Declare(asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-34-2
)

FetchContent_MakeAvailable(asio)

# Create interface library
FetchContent_GetProperties(asio SOURCE_DIR ASIO_SOURCE_DIR)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${ASIO_SOURCE_DIR}/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)