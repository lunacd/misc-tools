add_executable(image-renamer
    image-renamer.cpp
)

find_package(TinyEXIF REQUIRED)
target_link_libraries(image-renamer PRIVATE
    TinyEXIF::TinyEXIF
)
