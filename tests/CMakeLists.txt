find_package(Catch2 3 REQUIRED)

list(APPEND TEST
    ${CMAKE_CURRENT_LIST_DIR}/cpuTest.cpp
)
add_executable(tests ${TEST})
target_link_libraries(tests PRIVATE 8080_lib Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)