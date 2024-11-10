# Dependencies
find_package(fmt REQUIRED)
find_package(pugixml REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(marisa REQUIRED IMPORTED_TARGET marisa)
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Widgets Concurrent)
find_package(ImageMagick REQUIRED COMPONENTS Magick++)

# Projects
add_subdirectory(libutil)
add_subdirectory(liblunacd-qml)
add_subdirectory(scrape-autocomplete)
add_subdirectory(nfo-editor)
add_subdirectory(image-transformer)