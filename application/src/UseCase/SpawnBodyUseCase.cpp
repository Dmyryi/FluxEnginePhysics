#include "../../include/application/UseCase/SpawnBodyUseCase.h"
#include "../../../domain/include/domain/BodyCreator.h"

namespace Flux::Application {

    SpawnBodyUseCase::SpawnBodyUseCase(std::vector<std::unique_ptr<Domain::PhysicsBody>>& bodies)
        : m_bodies(bodies) {
    }

    std::uint64_t SpawnBodyUseCase::execute(const Domain::BodyCreator& creator, const Domain::Vector2D& position) {
        const std::uint64_t id = m_nextId++;

        auto newBody = creator.create(id, position);
        m_bodies.push_back(std::move(newBody));

        return id;
    }

}