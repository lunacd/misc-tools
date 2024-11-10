qt_standard_project_setup(REQUIRES 6.5)

set(sources
    ImageTransformer.cpp
    ImageTransformerQtBridge.cpp
)

if (WIN32)
    qt_add_executable(image-transformer
        WIN32
        ${sources}
    )
else ()
    qt_add_executable(image-transformer
        ${sources}
    )
endif ()

target_sources(image-transformer PRIVATE FILE_SET HEADERS FILES
    ImageTransformerQtBridge.hpp
)

qt_add_qml_module(image-transformer
    URI com.lunacd.imageTransformer
    QML_FILES
        qml/ImageTransformer.qml
    RESOURCE_PREFIX /
)
qt_import_qml_plugins(image-transformer)

target_link_libraries(image-transformer PRIVATE Qt6::Qml Qt6::Quick Qt6::Widgets Qt6::Concurrent)
target_link_libraries(image-transformer PRIVATE ImageMagick::Magick++)
target_link_libraries(image-transformer PRIVATE fmt::fmt)
target_link_libraries(image-transformer PRIVATE lunacd-qmlplugin)
