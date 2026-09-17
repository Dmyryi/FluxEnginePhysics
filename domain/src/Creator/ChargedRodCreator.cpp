#include "../../include/domain/Creator/ChargedRodCreator.h"
#include "../../include/domain/Tools/ChargedRod.h"

namespace Flux::Domain {

	std::unique_ptr<PhysicsBody> ChargedRodCreator::create(std::uint64_t id, const Vector2D& position) const {
		constexpr float totalCharge = 1.0f;
		constexpr float len = 6.0f;
		return std::make_unique<ChargedRod>(id, "ChargedRod", position, len, m_charge);
	}

}