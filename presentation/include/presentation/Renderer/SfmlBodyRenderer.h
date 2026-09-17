
#pragma once
#include <SFML/Graphics.hpp>
#include "domain/IBodyVisitor.h"
#include "domain/Tools/PointCharge.h"
#include "domain/Tools/ChargedRod.h"

namespace Flux::Presentation {

    class SfmlBodyRenderer : public Domain::IBodyVisitor {
    public:
        explicit SfmlBodyRenderer(sf::RenderTarget& target)
            : m_target(target) {
        }

        void visit(const Domain::PointCharge& pc) override {
            const auto& pos = pc.getPosition();
            const float r = pc.getRadius();

            sf::CircleShape circle(r);
            circle.setOrigin({ r, r });
            circle.setPosition({ pos.x, pos.y });
            circle.setFillColor(pc.getCharge() >= 0.0f ? sf::Color(235, 64, 52)
                : sf::Color(52, 131, 235));
            m_target.draw(circle);
        }

        void visit(const Domain::ChargedRod& rod) override {
            const auto& pos = rod.getPosition();
            const float len = rod.getLength();

            sf::RectangleShape rect({ len, 6.0f });
            rect.setOrigin({ len * 0.5f, 3.0f });
            rect.setPosition({ pos.x, pos.y });
            rect.setRotation(sf::radians(rod.getAngle()));
            rect.setFillColor(sf::Color(250, 180, 50));
            m_target.draw(rect);
        }

    private:
        sf::RenderTarget& m_target;
    };

}