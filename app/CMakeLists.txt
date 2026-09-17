add_executable(FluxEnginePhysics main.cpp)

target_link_libraries(FluxEnginePhysics PRIVATE
application
    domain
    presentation
    SFML::Graphics
    SFML::Window
    SFML::System
    ImGui-SFML::ImGui-SFML
)

target_compile_features(FluxEnginePhysics PRIVATE cxx_std_20)