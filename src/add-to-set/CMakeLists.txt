add_executable(add-to-set main.cpp)

find_package(pugixml REQUIRED)
target_link_libraries(add-to-set PRIVATE pugixml::pugixml)
