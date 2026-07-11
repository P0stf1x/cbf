cmake_minimum_required(VERSION 3.15)
project(cbf)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

add_compile_options(-Wall -Wextra)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/parser.cpp
    src/runner.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

include(CTest)
enable_testing()
