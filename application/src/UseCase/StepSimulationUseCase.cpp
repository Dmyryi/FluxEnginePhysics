#include "application/UseCase/StepSimulationUseCase.h"
#include "domain/PhysicSolver.h"

namespace Flux::Application {

    void StepSimulationUseCase::execute(
        std::vector<std::unique_ptr<Domain::PhysicsBody>>& bodies,
        float dt,
        float coulombK,
        float dragCoeff
    ) {
        if (dt <= 0.0f || bodies.empty()) return;

        const size_t count = bodies.size();

        for (auto& body : bodies) {
            if (!body->isPinned()) {
                Domain::Vector2D drag = Domain::PhysicSolver::calculateAirResistance(
                    body->getVelocity(),
                    dragCoeff
                );
                body->applyForce(drag);
            }
        }

       
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                auto& b1 = bodies[i];
                auto& b2 = bodies[j];

                if (b1->isPinned() && b2->isPinned()) continue;

                auto elements1 = b1->getChargeElements();
                auto elements2 = b2->getChargeElements();

                Domain::
                    Vector2D forceOnB2 = Domain::PhysicSolver::calculateForceBetweenBodies(elements1, elements2, coulombK);

                if (!b2->isPinned()) b2->applyForce(forceOnB2);
                if (!b1->isPinned()) b1->applyForce(forceOnB2 * -1.0f);
        }

        for (auto& body : bodies) {
            body->integrate(dt);
        }
    }

}