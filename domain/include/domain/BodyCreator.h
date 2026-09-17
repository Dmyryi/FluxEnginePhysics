#pragma once
#include "domain/PhysicsBody.h"
#include "domain/Vector2D.h"
#include <memory>
#include <cstdint>


namespace Flux ::Domain {
	class BodyCreator {
	public:
		virtual ~BodyCreator() = default;

		virtual std::unique_ptr<PhysicsBody> create(std::uint64_t id, const Vector2D& position) const = 0;
	};
}