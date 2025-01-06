# Dependencies
find_package(fmt REQUIRED)
find_package(pugixml REQUIRED)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)

# Projects
add_subdirectory(libutil)
add_subdirectory(nfo-editor)
add_subdirectory(image-transformer)
add_subdirectory(oa-relay)
