add_executable(scrape-autocomplete
    scrapeAutocomplete.cpp
)

find_package(pugixml REQUIRED)
target_link_libraries(scrape-autocomplete PRIVATE pugixml::pugixml)

find_package(fmt REQUIRED)
target_link_libraries(scrape-autocomplete PRIVATE fmt::fmt)

target_link_libraries(scrape-autocomplete PRIVATE util)