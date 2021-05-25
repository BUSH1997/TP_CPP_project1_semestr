cmake_minimum_required(VERSION 3.15)
project(prog)

set(CMAKE_CXX_STANDARD 14)

set(STATIC_LIB_SOURCE_FILES project/parser/src/parser.cpp)
add_library(parser_lib STATIC ${STATIC_LIB_SOURCE_FILES})
target_include_directories(parser_lib PUBLIC project/parser/include)

set(STATIC_LIB_SOURCE_FILES project/wrapper/src/DBwrapper.cpp)
add_library(wrapper_lib STATIC ${STATIC_LIB_SOURCE_FILES})
target_link_libraries(wrapper_lib mysqlclient)
target_include_directories(wrapper_lib PUBLIC project/wrapper/include)

set(STATIC_LIB_SOURCE_FILES project/businessLogic/src/handlers.cpp)
add_library(handlers_lib STATIC ${STATIC_LIB_SOURCE_FILES})
target_include_directories(handlers_lib PUBLIC project/businessLogic/include)
target_link_libraries(handlers_lib wrapper_lib)
target_link_libraries(handlers_lib parser_lib)


set(CMAKE_INCLUDE_PATH /usr/lib/openssl)

set(OPENSSL_USE_STATIC_LIBS ON)

find_package(OpenSSL)
find_library(OpenSSL_LIB  libcrypto.a libssl.a)

if (OPENSSL_FOUND)
    message (STATUS "OPENSSL found")
    message (STATUS "OpenSSL Include directories:" OPENSSL_INCLUDE_DIR)
else()
    message (FATAL_ERROR "OpenSSL Not found.")
endif()

include_directories(${OPENSSL_INCLUDE_DIR})

target_link_libraries(handlers_lib ${OPENSSL_LIBRARIES})

set (BOOST_ROOT "/usr/local/boost_1_76_0")
set (Boost_NO_SYSTEM_PATHS OFF)

find_package (Boost REQUIRED)

if(Boost_FOUND)
    message("Boost is OK: ${Boost_INCLUDE_DIRS}")
    message("Boost is OK: ${Boost_LIBRARIES}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    target_link_libraries(parser_lib ${Boost_LIBRARIES})
endif()


enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})


