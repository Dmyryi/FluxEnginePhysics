#include "presentation/Camera2D.h"
#include <cmath>

namespace Flux::Presentation {

    Camera2D::Camera2D(float width, float height)
        : m_view(sf::FloatRect({ -width * 0.5f, -height * 0.5f }, { width, height })), m_zoomFactor(1.0f) {
    }

    void Camera2D::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Middle || mousePressed->button == sf::Mouse::Button::Right) {
                m_isPanning = true;
                m_lastMousePos = window.mapPixelToCoords(mousePressed->position, m_view);
            }
        }
        else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseReleased->button == sf::Mouse::Button::Middle || mouseReleased->button == sf::Mouse::Button::Right) {
                m_isPanning = false;
            }
        }
        else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
            if (m_isPanning) {
                sf::Vector2f currentMousePos = window.mapPixelToCoords(mouseMoved->position, m_view);
                sf::Vector2f delta = m_lastMousePos - currentMousePos;
                m_view.move(delta);
                m_lastMousePos = window.mapPixelToCoords(mouseMoved->position, m_view);
            }
        }
        else if (const auto* wheelScrolled = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (wheelScrolled->wheel == sf::Mouse::Wheel::Vertical) {
                float zoomRatio = (wheelScrolled->delta > 0.0f) ? 0.9f : 1.1f;
                if ((m_zoomFactor * zoomRatio > 0.01f) && (m_zoomFactor * zoomRatio < 50.0f)) {
                    sf::Vector2i mousePixel = wheelScrolled->position;
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

        float gridSize = 50.0f;
        while (gridSize / m_zoomFactor < 25.0f) gridSize *= 2.0f;
        while (gridSize / m_zoomFactor > 150.0f) gridSize /= 2.0f;

        float startX = std::floor(left / gridSize) * gridSize;
        float startY = std::floor(top / gridSize) * gridSize;

        sf::VertexArray lines(sf::PrimitiveType::Lines);

        // Вертикальные линии
        for (float x = startX; x <= right; x += gridSize) {
            sf::Color color = (std::abs(x) < 0.001f) ? sf::Color(120, 180, 255, 220) : sf::Color(60, 60, 75, 120);
            lines.append(sf::Vertex{ .position = sf::Vector2f(x, top), .color = color });
            lines.append(sf::Vertex{ .position = sf::Vector2f(x, bottom), .color = color });
        }

        // Горизонтальные линии
        for (float y = startY; y <= bottom; y += gridSize) {
            sf::Color color = (std::abs(y) < 0.001f) ? sf::Color(255, 120, 120, 220) : sf::Color(60, 60, 75, 120);
            lines.append(sf::Vertex{ .position = sf::Vector2f(left, y), .color = color });
            lines.append(sf::Vertex{ .position = sf::Vector2f(right, y), .color = color });
        }

        target.draw(lines);
    }

}