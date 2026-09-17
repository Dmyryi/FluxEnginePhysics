#pragma once

#include "domain/PhysicsBody.h"
#include "domain/ChargeElement.h"
#include <vector>
#include <cstdint>
#include <string>

namespace Flux::Domain {

    class IBodyVisitor;

    class PointCharge : public PhysicsBody {
    private:
        float m_charge{ 0.0f };
        float m_radius{ 6.0f };

    public:
        PointCharge() = default;
        PointCharge(std::uint64_t id, std::string name, const Vector2D& pos, float charge, float radius = 6.0f);

        void accept(IBodyVisitor& visitor) const override;

        float getCharge() const noexcept { return m_charge; }
        void setCharge(float q) noexcept { m_charge = q; }

        float getRadius() const noexcept { return m_radius; }
        void setRadius(float r) noexcept;

        std::vector<ChargeElement> getChargeElements() const override {
            return {
                { getPosition(), m_charge }
            };
        }
    };

}