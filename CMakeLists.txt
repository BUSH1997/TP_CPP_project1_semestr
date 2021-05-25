CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

add_subdirectory(kust_libs)

add_executable(sync_server sync_server.cpp)
        
set (BOOST_ROOT "/usr/local/boost_1_76_0")
set (Boost_NO_SYSTEM_PATHS OFF)


find_package (Boost 1.76.0 REQUIRED)

if(Boost_FOUND)
    message("Boost is OK: ${Boost_INCLUDE_DIRS}")
    message("Boost is OK: ${Boost_LIBRARIES}")
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    include_directories(SYSTEM ${Boost_LIBRARY_DIRS})
    target_link_libraries(sync_server PRIVATE ${Boost_LIBRARIES})
    find_package(Boost 1.76.0 COMPONENTS thread REQUIRED)
    target_link_libraries(sync_server PRIVATE Boost::thread)
endif()


target_link_libraries(sync_server PRIVATE parser_lib)
target_link_libraries(sync_server PRIVATE wrapper_lib)
target_link_libraries(sync_server PRIVATE handlers_lib)
