cmake_minimum_required(VERSION 3.26)
project(misc-tools LANGUAGES CXX)

# Force on UTF-8 mode on MSVC as Qt requires it
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/utf-8 /EHsc")
endif ()
if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "/utf-8")
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Widgets Concurrent)
qt_standard_project_setup(REQUIRES 6.5)
set(QT_QML_GENERATE_QMLLS_INI ON)

qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0005 NEW)

add_subdirectory(src)

include(CTest)
add_subdirectory(tests)