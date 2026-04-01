cmake_minimum_required(VERSION 3.20)

project(cnerium_router
    VERSION 0.1.0
    DESCRIPTION "Cnerium Router - HTTP router for the Cnerium web framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ========================
# Options
# ========================
option(CNERIUM_ROUTER_BUILD_TESTS "Build router tests" ON)
option(CNERIUM_ROUTER_BUILD_EXAMPLES "Build router examples" ON)

# ========================
# Global settings
# ========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ========================
# Dependencies
# ========================
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
    include("${CMAKE_CURRENT_SOURCE_DIR}/.vix/vix_deps.cmake")
endif()

if(NOT TARGET cnerium::http AND NOT TARGET cnerium_http)
    find_package(cnerium_http CONFIG QUIET)
    find_package(http CONFIG QUIET)
endif()

set(CNERIUM_ROUTER_HTTP_TARGET "")
if(TARGET cnerium::http)
    set(CNERIUM_ROUTER_HTTP_TARGET cnerium::http)
elseif(TARGET cnerium_http)
    set(CNERIUM_ROUTER_HTTP_TARGET cnerium_http)
endif()

if(CNERIUM_ROUTER_HTTP_TARGET STREQUAL "")
    message(FATAL_ERROR
        "cnerium http target not found.\n"
        "Install dependencies with Vix:\n"
        "  vix add @cnerium/http\n"
        "  vix install"
    )
endif()

# ========================
# Library (header-only)
# ========================
add_library(cnerium_router INTERFACE)
add_library(cnerium::router ALIAS cnerium_router)

target_compile_features(cnerium_router INTERFACE cxx_std_20)

target_link_libraries(cnerium_router
    INTERFACE
        ${CNERIUM_ROUTER_HTTP_TARGET}
)

target_include_directories(cnerium_router
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# ========================
# Compiler options
# ========================
if(MSVC)
    target_compile_options(cnerium_router INTERFACE /W4 /permissive-)
else()
    target_compile_options(cnerium_router INTERFACE -Wall -Wextra -Wpedantic)
endif()

# ========================
# Target properties
# ========================
set_target_properties(cnerium_router PROPERTIES
    EXPORT_NAME router
)

# ========================
# Examples
# ========================
if(CNERIUM_ROUTER_BUILD_EXAMPLES AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
endif()

# ========================
# Tests
# ========================
if(CNERIUM_ROUTER_BUILD_TESTS)
    include(CTest)
    enable_testing()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_basic.cpp")
        add_executable(cnerium_router_test_basic tests/test_basic.cpp)
        target_link_libraries(cnerium_router_test_basic PRIVATE cnerium::router)
        target_compile_features(cnerium_router_test_basic PRIVATE cxx_std_20)

        if(MSVC)
            target_compile_options(cnerium_router_test_basic PRIVATE /W4 /permissive-)
        else()
            target_compile_options(cnerium_router_test_basic PRIVATE -Wall -Wextra -Wpedantic)
        endif()

        add_test(NAME cnerium_router.basic COMMAND cnerium_router_test_basic)
    endif()
endif()

# ========================
# Summary
# ========================
message(STATUS "------------------------------------------------------")
message(STATUS "cnerium::router configured (${PROJECT_VERSION})")
message(STATUS "HTTP target:     ${CNERIUM_ROUTER_HTTP_TARGET}")
message(STATUS "Include dir:     ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "Build tests:     ${CNERIUM_ROUTER_BUILD_TESTS}")
message(STATUS "Build examples:  ${CNERIUM_ROUTER_BUILD_EXAMPLES}")
message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "------------------------------------------------------")
