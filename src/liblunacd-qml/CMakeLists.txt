qt_standard_project_setup(REQUIRES 6.5)

qt_add_library(lunacd-qml STATIC)
qt_add_qml_module(lunacd-qml
    URI com.lunacd.qml
    VERSION 1.0
    QML_FILES
        qml/LunacdButton.qml
        qml/LunacdLabeledInput.qml
        qml/LunacdSuggestionBox.qml
        qml/LunacdSuggestionItem.qml
        qml/LunacdSuggestionPanel.qml
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/com/lunacd/qml
)

target_sources(lunacd-qml PRIVATE
    LunacdQmlAutocomplete.cpp
)
target_sources(lunacd-qml PRIVATE FILE_SET HEADERS FILES
    LunacdQmlAutocomplete.hpp
)

target_link_libraries(lunacd-qml PRIVATE PkgConfig::marisa)
target_link_libraries(lunacd-qml PRIVATE fmt::fmt)
target_link_libraries(lunacd-qml PRIVATE util)
target_link_libraries(lunacd-qml PRIVATE Qt6::Quick Qt6::Qml)
