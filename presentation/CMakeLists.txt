add_library(presentation STATIC
    src/placeholder.cpp
    include/presentation/Camera2D.h
    src/Camera2D.cpp
    include/presentation/Renderer/SfmlBodyRenderer.h
    include/presentation/Inspector/ImGuiInspectorVisitor.h
)

target_include_directories(presentation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR} # קעמבû גטהועü ץוהונא imgui.h
)

target_link_libraries(presentation PUBLIC
    domain
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML
)

target_compile_features(presentation PUBLIC cxx_std_20)