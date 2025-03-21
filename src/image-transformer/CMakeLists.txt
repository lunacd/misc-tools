if (WIN32 AND (NOT CMAKE_BUILD_TYPE STREQUAL Debug))
    qt_add_executable(image-transformer
        WIN32
    )
else ()
    qt_add_executable(image-transformer
    )
endif ()

target_sources(image-transformer PRIVATE
    ImageTransformer.cpp
    ImageTransformerQtBridge.cpp
)

target_sources(image-transformer PRIVATE FILE_SET HEADERS FILES
    ImageTransformerQtBridge.hpp
)

qt_add_qml_module(image-transformer
    URI image-transformer
    VERSION 1.0
    QML_FILES
        ImageTransformer.qml
    RESOURCE_PREFIX /qt/qml/
    IMPORTS TARGET ui
)
qt_import_qml_plugins(image-transformer)

find_package(ImageMagick REQUIRED COMPONENTS Magick++)

target_link_libraries(image-transformer PRIVATE
    util
    uiplugin
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
    Qt6::Concurrent
    ImageMagick::Magick++
)
