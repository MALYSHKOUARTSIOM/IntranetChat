cmake_minimum_required(VERSION 3.29)
project(intranetChat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/programs/qt/6.7.2/msvc2019_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(intranetChat main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui)
target_link_libraries(intranetChat
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    add_custom_command(TARGET intranetChat POST_BUILD
            COMMAND Qt6::windeployqt
            ARGS $<TARGET_FILE:intranetChat>
    )
endif ()
