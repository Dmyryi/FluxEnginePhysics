#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include "Camera2D.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "FluxEngine Physics");
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) {
        return -1;
    }

    sf::Clock deltaClock;
    Flux::Presentation::Camera2D camera(1280.0f, 720.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            // Камера зумит и двигает поле, только когда курсор не на окне ImGui
            if (!ImGui::GetIO().WantCaptureMouse) {
                camera.handleEvent(event, window);
            }

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Обновляем ImGui на каждый кадр
        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        // Простое окно со статистикой
        ImGui::Begin("Engine Inspector");
        ImGui::Text("FPS: %.1f", 1.0f / dt.asSeconds());
        ImGui::Text("Controls: MMB (Drag) | Scroll (Zoom)");
        ImGui::End();

        // Очистка экрана
        window.clear(sf::Color(25, 26, 32));

        // 1. Рендерим физический мир и сетку через камеру
        window.setView(camera.getView());
        camera.renderGrid(window);

        // 2. Рендерим интерфейс в экранных координатах
        window.setView(window.getDefaultView());
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}