add_executable(nfo-editor)

target_sources(nfo-editor PRIVATE
    NfoEditor.cpp
    NfoEditorXml.cpp
    NfoEditorController.cpp
    NfoEditorAutocomplete.cpp
    NfoEditorStaticController.cpp
    NfoEditorExpiringMap.cpp
)

target_sources(nfo-editor PRIVATE FILE_SET HEADERS FILES
    NfoEditorXml.hpp
    NfoEditorController.hpp
    NfoEditorAutocomplete.hpp
    NfoEditorStaticController.hpp
    NfoEditorExpiringMap.hpp
    dto/NfoEditorAutocompleteResponse.hpp
    dto/NfoEditorSaveToNfoRequest.hpp
    dto/NfoEditorSaveToNfoResponse.hpp
)

target_link_libraries(nfo-editor PRIVATE fmt::fmt)
target_link_libraries(nfo-editor PRIVATE pugixml::pugixml)
target_link_libraries(nfo-editor PRIVATE util)

find_package(marisa REQUIRED)
target_link_libraries(nfo-editor PRIVATE marisa::marisa)

find_package(oatpp REQUIRED)
target_link_libraries(nfo-editor PRIVATE oatpp::oatpp)
