CMAKE_MINIMUM_REQUIRED(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sync_client
        sync_client.cpp)

find_package(Boost 1.71.0 REQUIRED)
target_link_libraries(sync_client PRIVATE ${Boost_LIBRARIES})

find_package(Boost COMPONENTS thread REQUIRED)
target_link_libraries(sync_client PRIVATE Boost::thread)
target_link_libraries(sync_client PRIVATE parser)
