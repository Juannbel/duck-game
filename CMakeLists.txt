cmake_minimum_required(VERSION 3.13)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(duck_game
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/Juannbel/duck-game"
    LANGUAGES CXX
    )

option(DUCK_GAME_TESTS "Enable / disable tests." ON)
option(DUCK_GAME_CLIENT "Enable / disable client program." ON)
option(DUCK_GAME_SERVER "Enable / disable server program." ON)
option(DUCK_GAME_EDITOR "Enable / disable editor program." ON)
option(DUCK_GAME_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

find_package(yaml-cpp REQUIRED)

add_library(duck_game_common STATIC)

add_subdirectory(common/)

target_link_libraries(duck_game_common PUBLIC yaml-cpp)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(duck_game_common ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} FALSE)

target_include_directories(duck_game_common PUBLIC .)

set(CONFIG_PATH "${PROJECT_SOURCE_DIR}/config.yaml")
set(DATA_PATH "${PROJECT_SOURCE_DIR}/client/data")
set(MAPS_PATH "${PROJECT_SOURCE_DIR}/server/maps")

if(DEFINED CUSTOM_CONFIG_PATH)
    set(CONFIG_PATH "${CUSTOM_CONFIG_PATH}")
endif()

if(DEFINED CUSTOM_DATA_PATH)
    set(DATA_PATH "${CUSTOM_DATA_PATH}")
endif()

if(DEFINED CUSTOM_MAPS_PATH)
    set(MAPS_PATH "${CUSTOM_MAPS_PATH}")
endif()

add_definitions(-DCONFIG_PATH="${CONFIG_PATH}")
add_definitions(-DDATA_PATH="${DATA_PATH}")
add_definitions(-DMAPS_PATH="${MAPS_PATH}")

if(DUCK_GAME_CLIENT OR DUCK_GAME_EDITOR)
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
if(DUCK_GAME_CLIENT)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    add_executable(duck_game_client)

    add_dependencies(duck_game_client duck_game_common SDL2pp::SDL2pp)

    add_subdirectory(client)

    set_project_warnings(duck_game_client ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} FALSE)

    target_include_directories(duck_game_client PRIVATE ${libSDL2pp_SOURCE_DIR} ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS})

    target_link_libraries(duck_game_client duck_game_common SDL2pp::SDL2pp yaml-cpp pre_lobby lobby Qt${QT_VERSION_MAJOR}::Widgets)
endif()

if(DUCK_GAME_SERVER)
    add_executable(duck_game_server)

    add_dependencies(duck_game_server duck_game_common)

    add_subdirectory(server)

    set_project_warnings(duck_game_server ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} FALSE)

    find_package(yaml-cpp REQUIRED)

    target_link_libraries(duck_game_server PRIVATE yaml-cpp duck_game_common)
endif()

if(DUCK_GAME_EDITOR)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    add_executable(duck_game_editor)
    add_dependencies(duck_game_editor duck_game_common)
    add_subdirectory(editor)

    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set_project_warnings(duck_game_editor ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} FALSE)

    find_package(yaml-cpp REQUIRED)

    target_link_libraries(duck_game_editor
        PRIVATE
        Qt5::Widgets
        yaml-cpp
        duck_game_common
    )

endif()


# Testing section
# ---------------

if(DUCK_GAME_TESTS)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

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

    add_executable(duck_game_client_tests)
    add_executable(duck_game_server_tests)

    add_dependencies(duck_game_client_tests duck_game_common)
    add_dependencies(duck_game_server_tests duck_game_common)

    target_include_directories(duck_game_client_tests PUBLIC .)
    target_include_directories(duck_game_server_tests PUBLIC .)

    add_subdirectory(tests/)

    set_project_warnings(duck_game_client_tests ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} TRUE)
    set_project_warnings(duck_game_server_tests ${DUCK_GAME_MAKE_WARNINGS_AS_ERRORS} TRUE)

    target_include_directories(duck_game_client_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    target_include_directories(duck_game_server_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        )

    target_link_libraries(duck_game_client_tests
        duck_game_common
        GTest::gtest_main
    )

    target_link_libraries(duck_game_server_tests
        duck_game_common
        GTest::gtest_main
    )
endif()
