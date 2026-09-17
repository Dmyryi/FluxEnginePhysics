#include "domain/Tools/ChargedRod.h"
#include "domain/IBodyVisitor.h"
#include <algorithm>
#include <cmath>

namespace Flux::Domain {

    ChargedRod::ChargedRod(std::uint64_t id, std::string name, const Vector2D& pos, float len, float q, float angle)
        : PhysicsBody(id, std::move(name), pos, 10.0f), m_totalCharge(q), m_angle(angle) {
        setLength(len);
    }

    void ChargedRod::setLength(float len) noexcept {
        if (len > 0.1f) {
            m_length = len;
        }
    }

    void ChargedRod::accept(IBodyVisitor& visitor) const {
        visitor.visit(*this);
    }

    std::vector<ChargeElement> ChargedRod::getChargeElements() const {
        constexpr int minSegments = 4;
        constexpr int maxSegments = 32;
        constexpr float pixLen = 10.0f;

        int segments = std::clamp(static_cast<int>(m_length / pixLen), minSegments, maxSegments);
        float dq = m_totalCharge / static_cast<float>(segments);

        Vector2D dir{ std::cos(m_angle), std::sin(m_angle) };

        Vector2D startPos = getPosition() - dir * (m_length * 0.5f);
        float step = m_length / static_cast<float>(segments);

        std::vector<ChargeElement> elements;
        elements.reserve(segments);

        for (int i = 0; i < segments; ++i) {
            float offset = step * (static_cast<float>(i) + 0.5f);
            Vector2D elementPos = startPos + dir * offset;

            elements.push_back(ChargeElement{ elementPos, dq });
        }

        return elements;
    }

}