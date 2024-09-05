set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS .)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Widgets)

set(sources
    NfoEditor.cpp
    NfoEditorQtBridge.cpp
    qml/NfoEditor.qrc
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

target_sources(nfo-editor PRIVATE FILE_SET CXX_MODULES FILES
    NfoEditorInterface.cppm
    NfoEditorXml.cppm
    NfoEditorAutocomplete.cppm
)

target_include_directories(nfo-editor PRIVATE .)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(nfo-editor PRIVATE fmt::fmt)

find_package(marisa REQUIRED)
target_link_libraries(nfo-editor PRIVATE marisa::marisa)

find_package(pugixml CONFIG REQUIRED)
target_link_libraries(nfo-editor PRIVATE pugixml::pugixml)

target_link_libraries(nfo-editor PRIVATE Qt6::Qml Qt6::Quick Qt6::Widgets)

qt_import_qml_plugins(nfo-editor)

target_link_libraries(nfo-editor PRIVATE util)
