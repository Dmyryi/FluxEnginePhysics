#pragma once

#include "domain/BodyCreator.h"

namespace Flux::Domain {

    class PointChargeCreator : public BodyCreator {
    private:
        float m_charge{ 1.0f };

    public:
        void setCharge(float q) noexcept { m_charge = q; }
        float getCharge() const noexcept { return m_charge; }

        std::unique_ptr<PhysicsBody> create(std::uint64_t id, const Vector2D& position) const override;
    };

}