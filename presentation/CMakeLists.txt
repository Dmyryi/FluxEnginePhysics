add_library(presentation STATIC
    src/placeholder.cpp
)
target_include_directories(presentation PUBLIC include)
target_link_libraries(presentation PUBLIC application SFML::Graphics ImGui-SFML::ImGui-SFML)