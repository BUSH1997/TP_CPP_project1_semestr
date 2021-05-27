set (BOOST_ROOT "/usr/local/boost_1_76_0")
set (Boost_NO_SYSTEM_PATHS OFF)

add_library(parser STATIC parser.cpp parser.h)

target_include_directories (parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package (Boost REQUIRED)

if(Boost_FOUND)
    message("Boost is OK: ${Boost_INCLUDE_DIRS}")
    message("Boost is OK: ${Boost_LIBRARIES}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    target_link_libraries(parser ${Boost_LIBRARIES})
endif()

