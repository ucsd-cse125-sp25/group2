include(FetchContent)

FetchContent_Declare(asio_external 
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-34-2
)

FetchContent_MakeAvailable(asio_external)