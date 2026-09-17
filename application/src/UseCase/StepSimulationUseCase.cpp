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

        // 2. Взаимодействие тел между собой (все уникальные пары i, j)
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                auto& b1 = bodies[i];
                auto& b2 = bodies[j];

                // Если оба зафиксированы, расчет пропускаем
                if (b1->isPinned() && b2->isPinned()) continue;

                auto elements1 = b1->getChargeElements();
                auto elements2 = b2->getChargeElements();

                // Сила, действующая со стороны b1 на b2
                Domain::// Сила, действующая НА b2 со стороны b1:
                    Vector2D forceOnB2 = Domain::PhysicSolver::calculateForceBetweenBodies(elements1, elements2, coulombK);

                if (!b2->isPinned()) b2->applyForce(forceOnB2);
                if (!b1->isPinned()) b1->applyForce(forceOnB2 * -1.0f); // противодействие
            }
        }

        // 3. Сдвигаем все тела на dt (интегрирование Ньютона)
        for (auto& body : bodies) {
            body->integrate(dt);
        }
    }

}