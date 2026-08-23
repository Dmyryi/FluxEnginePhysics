#pragma once
#include <SFML/Graphics.hpp>

namespace Flux::Presentation {

    class Camera2D {
    public:
        Camera2D(float width, float height);

        void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
        void renderGrid(sf::RenderTarget& target) const;

        const sf::View& getView() const { return m_view; }
        void resetSize(float width, float height) { m_view.setSize(width, height); }

    private:
        sf::View m_view;
        float m_zoomFactor;
        bool m_isPanning{ false };
        sf::Vector2f m_lastMousePos;
    };

}