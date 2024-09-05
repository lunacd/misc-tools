add_executable(scrape-autocomplete)

target_sources(scrape-autocomplete PRIVATE FILE_SET CXX_MODULES FILES
    scrapeAutocomplete.cppm
)

find_package(pugixml REQUIRED)
target_link_libraries(scrape-autocomplete PRIVATE pugixml::pugixml)

find_package(fmt REQUIRED)
target_link_libraries(scrape-autocomplete PRIVATE fmt::fmt)

target_link_libraries(scrape-autocomplete PRIVATE util)