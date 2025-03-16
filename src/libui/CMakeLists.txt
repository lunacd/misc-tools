qt_add_qml_module(ui
    URI libui
    VERSION 1.0
    STATIC
    QML_FILES
        LunacdButton.qml
        LunacdLabeledInput.qml
        LunacdSuggestionBox.qml
        LunacdSuggestionPanel.qml
        LunacdSuggestionItem.qml
    SOURCES
        LunacdAutocomplete.cpp
        LunacdAutocomplete.hpp
    RESOURCE_PREFIX /qt/qml
)

find_package(marisa REQUIRED)

target_link_libraries(ui PRIVATE util marisa::marisa)
