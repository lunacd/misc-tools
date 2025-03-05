qt_add_qml_module(ui
    URI "Lunacd"
    VERSION 1.0
    STATIC
    QML_FILES
        LunacdButton.qml
        LunacdLabeledInput.qml
        LunacdSuggestionBox.qml
        LunacdSuggestionPanel.qml
        LunacdSuggestionItem.qml
    RESOURCE_PREFIX /qt/qml
)

target_sources(ui PRIVATE
    LunacdAutocomplete.cpp
)

target_sources(ui PRIVATE FILE_SET HEADERS FILES
    LunacdAutocomplete.hpp
)

find_package(marisa REQUIRED)

target_link_libraries(ui PRIVATE util marisa::marisa)
