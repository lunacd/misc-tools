add_executable(image-transformer
    ImageTransformer.cpp
    ImageTransformerController.cpp
)
target_sources(image-transformer PRIVATE FILE_SET HEADERS FILES
    ImageTransformerController.hpp
)

target_link_libraries(image-transformer PRIVATE util)
target_link_libraries(image-transformer PRIVATE ImageMagick::Magick++)
target_link_libraries(image-transformer PRIVATE fmt::fmt)
