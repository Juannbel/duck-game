cmake_minimum_required(VERSION 3.16)

project(lobby VERSION 0.1 LANGUAGES CXX)

set(PROJECT_SOURCES
        lobby.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

add_library(lobby STATIC ${PROJECT_SOURCES})

target_include_directories(lobby 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lobby
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    taller_common
)