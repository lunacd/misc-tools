add_executable(image-transformer
    ImageTransformer.cpp
    ImageTransformerController.cpp
)
target_sources(image-transformer PRIVATE FILE_SET HEADERS FILES
    ImageTransformerController.hpp
)

target_link_libraries(image-transformer PRIVATE util)

find_package(ImageMagick REQUIRED)
target_link_libraries(image-transformer PRIVATE ImageMagick::Magick++)

find_package(fmt REQUIRED)
target_link_libraries(image-transformer PRIVATE fmt::fmt)

find_package(Boost REQUIRED)
target_include_directories(image-transformer PRIVATE ${Boost_INCLUDE_DIRS})
