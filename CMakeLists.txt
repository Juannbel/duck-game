cmake_minimum_required(VERSION 3.13)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

option(TALLER_TESTS "Enable / disable tests." ON)
option(TALLER_CLIENT "Enable / disable client program." ON)
option(TALLER_SERVER "Enable / disable server program." ON)
option(TALLER_EDITOR "Enable / disable editor program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

find_package(yaml-cpp REQUIRED)

# Define target: lib common between client, server and editor programs
# HEY TODO XXX: change the name to something that you like
add_library(taller_common STATIC)

# add the source files that make the lib
# HEY!! TODO XXX: add more subfolders here
add_subdirectory(common/)

# Añadir yaml-cpp como una dependencia de linkeo de taller_common
target_link_libraries(taller_common PUBLIC yaml-cpp)

# set compiler warnings to the targets
include(cmake/CompilerWarnings.cmake)
set_project_warnings(taller_common ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Say where the headers will be found required by library target
# Make this PUBLIC so any dependant linking lib will inherit this
# location (so our dependants will not have to explicitly request this)
target_include_directories(taller_common PUBLIC .)

set(CONFIG_PATH "${PROJECT_SOURCE_DIR}/config.yaml")
set(DATA_PATH "${PROJECT_SOURCE_DIR}/client/data")
set(SERVER_DATA_PATH "${PROJECT_SOURCE_DIR}/server/maps")

if(DEFINED CUSTOM_CONFIG_PATH)
    set(CONFIG_PATH "${CUSTOM_CONFIG_PATH}")
endif()

if(DEFINED CUSTOM_DATA_PATH)
    set(DATA_PATH "${CUSTOM_DATA_PATH}")
endif()

if(DEFINED CUSTOM_SERVER_DATA_PATH)
    set(SERVER_DATA_PATH "${CUSTOM_SERVER_DATA_PATH}")
endif()

add_definitions(-DCONFIG_PATH="${CONFIG_PATH}")
add_definitions(-DDATA_PATH="${DATA_PATH}")
add_definitions(-DSERVER_DATA_PATH="${SERVER_DATA_PATH}")

# Install libSDL2pp (for client or editor)
# HEY!! TODO XXX: add more dependendies (either for client, server or editor)
if(TALLER_CLIENT OR TALLER_EDITOR)
    include(FetchContent)

    set(SDL2PP_WITH_IMAGE YES)
    set(SDL2PP_WITH_MIXER YES)
    set(SDL2PP_WITH_TTF YES)

    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/cc198c9a5657048bee67ece82de620b2d5661084.zip
    )

    FetchContent_MakeAvailable(libSDL2pp)

endif()

# Program section
# ---------------
if(TALLER_CLIENT)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(taller_client)

    # Make them depend on taller_common lib
    add_dependencies(taller_client taller_common SDL2pp::SDL2pp)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(client)

    set_project_warnings(taller_client ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # add libSDL2pp's headers
    target_include_directories(taller_client PRIVATE ${libSDL2pp_SOURCE_DIR} ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

    # Link the taller_common lib target
    target_link_libraries(taller_client taller_common SDL2pp::SDL2pp yaml-cpp lobby Qt${QT_VERSION_MAJOR}::Widgets)
endif()

if(TALLER_SERVER)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_server)

    # Make them depend on taller_common lib
    add_dependencies(taller_server taller_common)

    # Add source files and enable warnings
    add_subdirectory(server)

    set_project_warnings(taller_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # Add yaml-cpp
    find_package(yaml-cpp REQUIRED)

    # Link the taller_common lib target
    target_link_libraries(taller_server PRIVATE yaml-cpp taller_common)
endif()

if(TALLER_EDITOR)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    add_executable(taller_editor)
    add_dependencies(taller_editor taller_common)
    add_subdirectory(editor)

    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set_project_warnings(taller_editor ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    find_package(yaml-cpp REQUIRED)

    target_link_libraries(taller_editor
        PRIVATE
        Qt5::Widgets
        yaml-cpp
        taller_common
    )

endif()


# Testing section
# ---------------

if(TALLER_TESTS)
    # Let's use google test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    # (borrowed from https://google.github.io/googletest/quickstart-cmake.html)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)

    # target for testing
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_tests)

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    add_dependencies(taller_tests taller_common)

    # Say where to find the xoz headers
    # HEY!! TODO XXX: add more subfolders here
    target_include_directories(taller_tests PUBLIC .)

    # Add the source files to the taller_tests target
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(tests/)

    # Set the compiler warnings to the tests target (but be a little lesser strict)
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    # Say where googletest headers can be found
    target_include_directories(taller_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    target_link_libraries(taller_tests
        taller_common
        GTest::gtest_main
    )
endif()
