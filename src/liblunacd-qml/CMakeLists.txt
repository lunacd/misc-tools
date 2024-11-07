find_package(Qt6 REQUIRED COMPONENTS Core Qml)

qt_standard_project_setup(REQUIRES 6.5)
set(CMAKE_AUTORCC ON)

qt_add_library(lunacd-qml STATIC)
qt_add_qml_module(lunacd-qml
    URI com.lunacd.qml
    VERSION 0.0.1
    QML_FILES
        LunacdButton.qml
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/com/lunacd/qml
)
