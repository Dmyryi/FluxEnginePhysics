#pragma once

#include "domain/PhysicsBody.h"
#include <vector>
#include <memory>
#include <cstdint>

namespace Flux::Domain {
    class BodyCreator;
}

namespace Flux::Application {

    class SpawnBodyUseCase {
    public:
        explicit SpawnBodyUseCase(std::vector<std::unique_ptr<Domain::PhysicsBody>>& bodies);

        [[nodiscard]] std::uint64_t execute(const Domain::BodyCreator& creator, const Domain::Vector2D& position);

    private:
        std::vector<std::unique_ptr<Domain::PhysicsBody>>& m_bodies;
        std::uint64_t m_nextId{ 1 };
    };

}