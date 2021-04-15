cmake_minimum_required(VERSION 3.8)
project(business)

set(CMAKE_CXX_STANDART 11)
set(CMAKE_CXX_STANDART_REQUIRED ON)

include_directories(project/include)

set(LIB_SOURCE_FILES project/src/jsonData.cpp project/include/jsonData.h)
add_library(jsonDatalib STATIC ${LIB_SOURCE_FILES})

set(LIB_SOURCE_FILES project/src/jsonParser.cpp project/include/jsonParser.h)
add_library(jsonParserlib STATIC ${LIB_SOURCE_FILES})
target_link_libraries(jsonParserlib jsonDatalib)

set(LIB_SOURCE_FILES project/src/wrapperBD.cpp project/include/wrapperBD.h)
add_library(wrapperBDlib STATIC ${LIB_SOURCE_FILES})
target_link_libraries(wrapperBDlib mysqlclient)

set(LIB_SOURCE_FILES project/src/handlers.cpp project/include/handlers.h)
add_library(handlerslib STATIC ${LIB_SOURCE_FILES})
target_link_libraries(handlerslib wrapperBDlib)
target_link_libraries(handlerslib jsonDatalib)
target_link_libraries(handlerslib mysqlclient)

add_executable(main_prog project/src/main.cpp)
target_link_libraries(main_prog wrapperBDlib)
target_link_libraries(main_prog handlerslib)
target_link_libraries(main_prog jsonParserlib)
target_link_libraries(main_prog jsonDatalib)

enable_testing()
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
message(${GTEST_LIBRARIES})
message(${GTEST_INCLUDE_DIRS})

#add_executable(tests tests/test.cpp)

#target_link_libraries(tests ${GTEST_LIBRARIES} -lpthread)
#target_link_libraries(tests matrix_lib)


