cmake_minimum_required(VERSION 3.1)
project(HomeWork1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -ggdb3")

include_directories(project/include)
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/include)

include(ExternalProject) # Подключает ExternalProject_Add
find_package(Git REQUIRED)

add_executable(server project/main.cpp)
target_link_libraries(server boost_system boost_thread pthread)

add_library(networking STATIC project/src/server.cpp
                              project/src/connection.cpp
                              project/src/handlers_chain.cpp)

option(ENABLE_COVERAGE "Build tests coverage statistic" OFF)
if (ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif ()

option(BUILD_TESTS "Build all tests." OFF) # Makes boolean 'test' available.
if (BUILD_TESTS)
    configure_file(CMakeLists.txt.in
            googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build)

    enable_testing()

    file(GLOB test_source tests/server_module.cpp
                          tests/connection_module.cpp
                          tests/handlers_module.cpp)

    foreach (file ${test_source})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" gtest_main)
        target_link_libraries("${name}_tests" networking)
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach ()
endif ()