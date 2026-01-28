find_package(Catch2 3 REQUIRED)

add_executable(tests ${CMAKE_CURRENT_LIST_DIR}/test.cpp)
target_link_libraries(tests PRIVATE 8080_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)