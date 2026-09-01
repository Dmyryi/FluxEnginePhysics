add_library(application STATIC
    src/placeholder.cpp
)
target_include_directories(application PUBLIC include)
target_link_libraries(application PUBLIC domain)