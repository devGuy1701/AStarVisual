cmake_minimum_required(VERSION 3.5)

add_subdirectory(./lib/googletest)
set(gtest_SOURCE_DIR, ./lib/googletest/)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(TEST_SOURCE_FILES runAllTests.cpp runAllTests.cpp CellTests.cpp ../Graph.cpp ../Graph.h GraphFixture.cpp)
add_executable(runTests ${TEST_SOURCE_FILES})
target_link_libraries(runTests gtest gtest_main core)

find_package(SFML 2 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(runTests sfml-graphics sfml-window sfml-system)
endif()