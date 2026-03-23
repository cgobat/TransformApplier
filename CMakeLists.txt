cmake_minimum_required(VERSION 3.20)

project(TransformApplier
    VERSION 3.0.0
    DESCRIPTION "Portable TransformApplier modernization scaffold"
    LANGUAGES CXX)

option(TA_BUILD_TESTS "Build tests" ON)
option(TA_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

add_library(ta_core
    src/config.cpp
    src/engine.cpp
    src/obs_file.cpp)

target_include_directories(ta_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

ta_set_project_warnings(ta_core ${TA_WARNINGS_AS_ERRORS})

if(MSVC)
    target_compile_definitions(ta_core PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

add_executable(ta_cli
    src/main.cpp)

target_link_libraries(ta_cli PRIVATE ta_core)

ta_set_project_warnings(ta_cli ${TA_WARNINGS_AS_ERRORS})

include(GNUInstallDirs)
install(TARGETS ta_cli
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(TA_BUILD_TESTS)
    include(CTest)
    if(BUILD_TESTING)
        add_executable(ta_tests tests/ta_tests.cpp)
        target_link_libraries(ta_tests PRIVATE ta_core)
        ta_set_project_warnings(ta_tests ${TA_WARNINGS_AS_ERRORS})
        add_test(NAME ta_tests COMMAND ta_tests)
    endif()
endif()
