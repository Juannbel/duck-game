cmake_minimum_required(VERSION 3.16)

project(pre_lobby VERSION 0.1 LANGUAGES CXX)

set(PROJECT_SOURCES
        pre_lobby.h
        prelobbymainwindow.cpp
        prelobbymainwindow.h
        prelobbymainwindow.ui
)

add_library(pre_lobby STATIC ${PROJECT_SOURCES})

target_include_directories(pre_lobby
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(pre_lobby
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    duck_game_common
)
