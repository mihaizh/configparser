cmake_minimum_required(VERSION 3.10)
project(configparser)

option(BUILD_EXAMPLE "Build the provided example." OFF)

add_library(${PROJECT_NAME}
    include/configparser.h
    include/value_parser.h
    include/value_type.h
    include/option_type.h
    include/section_type.h
    include/utils.h
    include/error_code.h
    include/token.h
    include/tokenizer.h
    src/configparser.cpp
    src/value_parser.cpp
    src/value_type.cpp
    src/option_type.cpp
    src/section_type.cpp
    src/utils.cpp
    src/tokenizer.cpp)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include)

if (BUILD_EXAMPLE)
    add_executable(${PROJECT_NAME}_example
        example/main.cpp)

    target_link_libraries(${PROJECT_NAME}_example
        PRIVATE
            ${PROJECT_NAME})
endif (BUILD_EXAMPLE)
