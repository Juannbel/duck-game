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
    
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMake source directory: ${CMAKE_SOURCE_DIR}")
include(../cmake/CompilerWarnings.cmake)


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
add_executable(server_protocol_tests
            server_protocol_tests.cpp
            ../server/server_protocol.cpp
            ../common/socket.cpp
            ../server/server_protocol.h
            ../common/socket.h
            ../common/lobby.h
            ../common/resolver.cpp
            ../common/resolver.h
            ../common/liberror.cpp
            ../common/liberror.h
            ../common/resolvererror.cpp
            ../common/resolvererror.h
)


# Say where to find the xoz headers
# HEY!! TODO XXX: add more subfolders here
target_include_directories(server_protocol_tests PUBLIC
            ${CMAKE_SOURCE_DIR}/../common
            ${CMAKE_SOURCE_DIR}/../server
            )

# Set the compiler warnings to the tests target (but be a little lesser strict)
set_project_warnings(server_protocol_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

# Say where googletest headers can be found
target_include_directories(server_protocol_tests
    PUBLIC
    ${gtest_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR}/include
    )

# HEY!! TODO XXX: I'm assuming that the code under testing is in 
# Change this if you have to.
target_link_libraries(server_protocol_tests
    GTest::gtest
    GTest::gtest_main
)

# HEY!! TODO XXX: Change the name of the executable to the one you like!
add_executable(client_protocol_tests
            client_protocol_tests.cpp
            ../client/client_protocol.cpp
            ../common/socket.cpp
            ../client/client_protocol.h
            ../common/socket.h
            ../common/lobby.h    
            ../common/commands.h                
           ../common/resolvererror.cpp
           ../common/resolvererror.h
           ../common/liberror.cpp
           ../common/liberror.h
           ../common/resolver.cpp
           ../common/resolver.h

)

# Say where to find the xoz headers
# HEY!! TODO XXX: add more subfolders here
target_include_directories(client_protocol_tests PUBLIC
            ${CMAKE_SOURCE_DIR}/../common
            ${CMAKE_SOURCE_DIR}/../client
            )

# Set the compiler warnings to the tests target (but be a little lesser strict)
set_project_warnings(client_protocol_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

# Say where googletest headers can be found
target_include_directories(client_protocol_tests
    PUBLIC
    ${gtest_SOURCE_DIR}/include
    ${gmock_SOURCE_DIR}/include
    )

# HEY!! TODO XXX: I'm assuming that the code under testing is in 
# Change this if you have to.
target_link_libraries(client_protocol_tests
    GTest::gtest
    GTest::gtest_main

)