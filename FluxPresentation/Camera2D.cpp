#include "Camera2D.h"
#include <cmath>

namespace Flux::Presentation {

    Camera2D::Camera2D(float width, float height)
        : m_view(sf::FloatRect(0.0f, 0.0f, width, height)), m_zoomFactor(1.0f) {
    }

    void Camera2D::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
            m_isPanning = true;
            m_lastMousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y }, m_view);
        }
        else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
            m_isPanning = false;
        }
        else if (event.type == sf::Event::MouseMoved && m_isPanning) {
            sf::Vector2f currentMousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }, m_view);
            sf::Vector2f delta = m_lastMousePos - currentMousePos;
            m_view.move(delta);
            m_lastMousePos = window.mapPixelToCoords({ event.mouseMove.x, event.mouseMove.y }, m_view);
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                float zoomRatio = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                if ((m_zoomFactor * zoomRatio > 0.05f) && (m_zoomFactor * zoomRatio < 20.0f)) {
                    sf::Vector2i mousePixel(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
                    sf::Vector2f beforeZoom = window.mapPixelToCoords(mousePixel, m_view);

                    m_view.zoom(zoomRatio);
                    m_zoomFactor *= zoomRatio;

                    sf::Vector2f afterZoom = window.mapPixelToCoords(mousePixel, m_view);
                    m_view.move(beforeZoom - afterZoom);
                }
            }
        }
    }

    void Camera2D::renderGrid(sf::RenderTarget& target) const {
        sf::Vector2f center = m_view.getCenter();
        sf::Vector2f size = m_view.getSize();

        float left = center.x - size.x * 0.5f;
        float right = center.x + size.x * 0.5f;
        float top = center.y - size.y * 0.5f;
        float bottom = center.y + size.y * 0.5f;

        const float baseGridSize = 50.0f;
        float gridSize = baseGridSize;
        while (gridSize / m_zoomFactor < 25.0f) gridSize *= 2.0f;
        while (gridSize / m_zoomFactor > 150.0f) gridSize /= 2.0f;

        float startX = std::floor(left / gridSize) * gridSize;
        float startY = std::floor(top / gridSize) * gridSize;

        sf::VertexArray lines(sf::Lines);

        for (float x = startX; x <= right; x += gridSize) {
            sf::Color color = (std::abs(x) < 0.001f) ? sf::Color(100, 100, 160, 200) : sf::Color(55, 55, 65, 120);
            lines.append(sf::Vertex(sf::Vector2f(x, top), color));
            lines.append(sf::Vertex(sf::Vector2f(x, bottom), color));
        }

        for (float y = startY; y <= bottom; y += gridSize) {
            sf::Color color = (std::abs(y) < 0.001f) ? sf::Color(100, 100, 160, 200) : sf::Color(55, 55, 65, 120);
            lines.append(sf::Vertex(sf::Vector2f(left, y), color));
            lines.append(sf::Vertex(sf::Vector2f(right, y), color));
        }

        target.draw(lines);
    }

}