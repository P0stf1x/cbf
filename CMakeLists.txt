cmake_minimum_required(VERSION 3.15)
project(cbf)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/parser.cpp
    src/runner.cpp
    src/compiler/aarch64.cpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE include)

include(CTest)
enable_testing()

if(WIN32)
  set(PYTHON_EXE python)
else()
  set(PYTHON_EXE python3)
endif()

add_test(
  NAME blackbox_tests
  COMMAND ${PYTHON_EXE} -m pytest
          ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_blackbox.py -v
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

set_tests_properties(blackbox_tests PROPERTIES
  ENVIRONMENT "BF_EXE=$<TARGET_FILE:${PROJECT_NAME}>"
  LABELS "blackbox"
)
