#pragma once

#include "domain/PhysicsBody.h"
#include "domain/ChargeElement.h"
#include <vector>
#include <cstdint>
#include <string>

namespace Flux::Domain {

    class IBodyVisitor;

    class ChargedRod : public PhysicsBody {
    private:
        float m_length{ 6.0f };
        float m_angle{ 0.0f };
        float m_totalCharge{ 0.0f };

    public:
        ChargedRod() = default;
        ChargedRod(std::uint64_t id, std::string name, const Vector2D& pos, float len, float q, float angle = 0.0f);

        void accept(IBodyVisitor& visitor) const override;

        std::vector<ChargeElement> getChargeElements() const override;

        float getLength() const noexcept { return m_length; }
        void setLength(float len) noexcept;

        float getAngle() const noexcept { return m_angle; }
        void setAngle(float rad) noexcept { m_angle = rad; }

        float getTotalCharge() const noexcept { return m_totalCharge; }
        void setTotalCharge(float q) noexcept { m_totalCharge = q; }
    };

}