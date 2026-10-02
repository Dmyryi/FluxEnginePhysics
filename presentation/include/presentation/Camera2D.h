#pragma once

#include <SFML/Graphics.hpp>

namespace Flux::Presentation {

    class Camera2D {
    public:
        Camera2D(float width, float height);

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void renderGrid(sf::RenderTarget& target) const;

        [[nodiscard]] const sf::View& getView() const noexcept { return m_view; }
        void setViewTo(sf::RenderTarget& target) const { target.setView(m_view); }
        void resetSize(float width, float height) { m_view.setSize({ width, height }); }

    private:
        sf::View m_view;
        float m_zoomFactor{ 1.0f };
        bool m_isPanning{ false };
        sf::Vector2f m_lastMousePos;
    };

}