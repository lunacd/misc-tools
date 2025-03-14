if (WIN32 AND (NOT CMAKE_BUILD_TYPE STREQUAL Debug))
    qt_add_executable(nfo-editor
        WIN32
    )
else()
    qt_add_executable(nfo-editor)
endif()

target_sources(nfo-editor PRIVATE
    NfoEditor.cpp
    NfoEditorXml.cpp
    NfoEditorQtBridge.cpp
)

target_sources(nfo-editor PRIVATE FILE_SET HEADERS FILES
    NfoEditorXml.hpp
    NfoEditorQtBridge.hpp
)

qt_add_qml_module(nfo-editor
    URI NfoEditor
    VERSION 1.0
    QML_FILES
        NfoEditor.qml
        NfoEditorLabeledListInput.qml
        NfoEditorRemovableItem.qml
    RESOURCES
        icons/plus.svg
        icons/x.svg
    RESOURCE_PREFIX /qt/qml/
    IMPORTS TARGET ui
)

target_include_directories(nfo-editor PRIVATE .)

find_package(fmt REQUIRED)
target_link_libraries(nfo-editor PRIVATE fmt::fmt)

find_package(pugixml REQUIRED)
target_link_libraries(nfo-editor PRIVATE pugixml::pugixml)

find_package(marisa REQUIRED)
target_link_libraries(nfo-editor PRIVATE marisa::marisa)

find_package(oatpp REQUIRED)
target_link_libraries(nfo-editor PRIVATE oatpp::oatpp)

find_package(stduuid REQUIRED)
target_include_directories(nfo-editor PRIVATE ${stduuid_INCLUDE_DIRS})

# GSL is a dependency of stduuid, but I really shouldn't have to do this
find_package(Microsoft.GSL)
target_link_libraries(nfo-editor PRIVATE Microsoft.GSL::GSL)

find_package(CURL REQUIRED)
target_link_libraries(nfo-editor PRIVATE CURL::libcurl)

target_link_libraries(nfo-editor PRIVATE
    util
    uiplugin
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
)
