#include "../../include/domain/Creator/PointChargeCreator.h"
#include "../../include/domain/Tools/PointCharge.h"

namespace Flux::Domain {

    std::unique_ptr<PhysicsBody> PointChargeCreator::create(std::uint64_t id, const Vector2D& position) const {
        return std::make_unique<PointCharge>(id, "PointCharge", position, m_charge);
    }

}