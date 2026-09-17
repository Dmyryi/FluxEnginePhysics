#include "../../include/domain/Tools/PointCharge.h"
#include "domain/IBodyVisitor.h"
namespace Flux::Domain {

    PointCharge::PointCharge(std::uint64_t id, std::string name, const Vector2D& pos, float charge, float radius)
        : PhysicsBody(id, std::move(name), pos, 1.0f), m_charge(charge) {
        setRadius(radius);
    }

    void PointCharge::setRadius(float r) noexcept {
        if (r > 0.1f) {
            m_radius = r;
        }
    }
    void PointCharge::accept(IBodyVisitor& visitor) const {
        visitor.visit(*this);
    }
}