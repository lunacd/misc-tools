qt_standard_project_setup(REQUIRES 6.5)

set(sources
    NfoEditor.cpp
    NfoEditorQtBridge.cpp
    NfoEditorXml.cpp
)

if (WIN32)
    qt_add_executable(nfo-editor
        WIN32
        ${sources}
    )
else ()
    qt_add_executable(nfo-editor
        ${sources}
    )
endif ()

target_sources(nfo-editor PRIVATE FILE_SET HEADERS FILES
    NfoEditorXml.hpp
)

qt_add_qml_module(nfo-editor
    URI com.lunacd.nfoEditor
    QML_FILES
        qml/LabeledListInput.qml
        qml/NfoEditor.qml
        qml/RemovableItem.qml
    RESOURCES
        icons/plus.svg
        icons/x.svg
)

qt_import_qml_plugins(nfo-editor)

target_link_libraries(nfo-editor PRIVATE fmt::fmt)
target_link_libraries(nfo-editor PRIVATE pugixml::pugixml)
target_link_libraries(nfo-editor PRIVATE Qt6::Qml Qt6::Quick Qt6::Widgets)
target_link_libraries(nfo-editor PRIVATE util)
target_link_libraries(nfo-editor PRIVATE lunacd-qmlplugin)
