cmake_minimum_required(VERSION 3.27)
project(misc-tools LANGUAGES CXX)

# Force on UTF-8 mode on MSVC as Qt requires it
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/utf-8 /EHsc")
endif ()
if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "/utf-8")
endif ()

add_subdirectory(src)

include(CTest)
add_subdirectory(tests)