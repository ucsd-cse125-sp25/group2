FetchContent_Declare(csv_parser
    GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser
    GIT_TAG master
)

FetchContent_MakeAvailable(csv_parser)

# Create interface library
FetchContent_GetProperties(csv_parser SOURCE_DIR PARSER_SOURCE_DIR)
add_library(csv_parser INTERFACE)
target_include_directories(csv_parser INTERFACE ${PARSER_SOURCE_DIR})