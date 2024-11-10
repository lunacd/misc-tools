add_executable(scrape-autocomplete
    ScrapeAutocomplete.cpp
)

target_link_libraries(scrape-autocomplete PRIVATE pugixml::pugixml)
target_link_libraries(scrape-autocomplete PRIVATE fmt::fmt)
target_link_libraries(scrape-autocomplete PRIVATE util)