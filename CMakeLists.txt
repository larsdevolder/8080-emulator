cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)

project(8080 LANGUAGES CXX)

find_package(Catch2 3 REQUIRED)

include(src/CMakeLists.txt)

add_library(8080_lib ${SRC})
target_include_directories(8080_lib PUBLIC include)
target_compile_options(8080_lib PRIVATE -Werror -Wall -Wextra)

add_executable(8080 src/main.cpp)
target_link_libraries(8080 PRIVATE 8080_lib)

include(tests/CMakeLists.txt)