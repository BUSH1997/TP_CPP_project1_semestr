cmake_minimum_required(VERSION 3.5)

project(client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Multimedia REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(parser)

find_package(Boost 1.71.0 REQUIRED)

include_directories(src)
include_directories(include)
set(PROJECT_SOURCES
        ./src/main.cpp

        ./include/mainwidget.h
        ./src/mainwidget.cpp

        ./include/contactWidget.h
        ./src/contactWidget.cpp

        ./include/audiooutput.h
        ./src/audiooutput.cpp

        ./include/audiorecorder.h
        ./src/audiorecorder.cpp

        ./include/authorizwindow.h
        ./src/authorizwindow.cpp
        ./src/authorizwindow.ui

        ./include/registrationwindow.h
        ./src/registrationwindow.cpp
        ./src/registrationwindow.ui

        ./include/serverConnection.h

        ./include/application.h
        ./src/application.cpp

)

#add_executable(client main.cpp)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(client
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(client SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(client PRIVATE Threads::Threads)
target_link_libraries(client PRIVATE parser)
target_link_libraries(client PRIVATE ${Boost_LIBRARIES})
