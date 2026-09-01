add_library(infrastructure STATIC
    src/placeholder.cpp
)
target_include_directories(infrastructure PUBLIC include)
target_link_libraries(infrastructure PUBLIC application)