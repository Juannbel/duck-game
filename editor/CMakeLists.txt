#Para correr en terminal o en el proyecto descomentar este target_source y comentar todas las de abajo.

target_sources(taller_editor
   PUBLIC
   main.cpp
   mainwindow.cpp
   mainwindow.h
   mainwindow.ui
    resources.qrc
)



# Para correr en qt creator comentar lo de arriba y descomentar lo de abajo.

# cmake_minimum_required(VERSION 3.16)

# project(TileEditor VERSION 0.1 LANGUAGES CXX)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# include_directories(${CMAKE_SOURCE_DIR}/../common)
# find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets yaml-cpp)
# find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
# add_definitions(-DSERVER_DATA_PATH="${PROJECT_SOURCE_DIR}/../server/maps")

# set(PROJECT_SOURCES
#         main.cpp
#         mainwindow.cpp
#         mainwindow.h
#         mainwindow.ui
#         resources.qrc




#         ${CMAKE_SOURCE_DIR}/../common/map_dto.h
#         ${CMAKE_SOURCE_DIR}/../common/map_loader.h
#         ${CMAKE_SOURCE_DIR}/../common/map_loader.cpp

# )

# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt_add_executable(TileEditor
#         MANUAL_FINALIZATION
#         ${PROJECT_SOURCES}
#     )
# # Define target properties for Android with Qt 6 as:
# #    set_property(TARGET TileEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
# #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
# else()
#     if(ANDROID)
#         add_library(TileEditor SHARED
#             ${PROJECT_SOURCES}
#         )
# # Define properties for Android with Qt 5 after find_package() calls as:
# #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#     else()
#         add_executable(TileEditor
#             ${PROJECT_SOURCES}
#         )
#     endif()
# endif()

# target_link_libraries(TileEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets yaml-cpp)

# # Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# # If you are developing for iOS or macOS you should consider setting an
# # explicit, fixed bundle identifier manually though.
# if(${QT_VERSION} VERSION_LESS 6.1.0)
#   set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TileEditor)
# endif()
# set_target_properties(TileEditor PROPERTIES
#     ${BUNDLE_ID_OPTION}
#     MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#     MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#     MACOSX_BUNDLE TRUE
#     WIN32_EXECUTABLE TRUE
# )

# include(GNUInstallDirs)
# install(TARGETS TileEditor
#     BUNDLE DESTINATION .
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )

# if(QT_VERSION_MAJOR EQUAL 6)
#     qt_finalize_executable(TileEditor)
# endif()
