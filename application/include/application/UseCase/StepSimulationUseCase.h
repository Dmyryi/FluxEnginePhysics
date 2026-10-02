#pragma once

#include "domain/PhysicsBody.h"
#include <vector>
#include <memory>

namespace Flux::Application {

    class StepSimulationUseCase {
    public:
       
        void execute(
            std::vector<std::unique_ptr<Domain::PhysicsBody>>& bodies,
            float dt,
            float coulombK = 50000.0f,
            float dragCoeff = 0.5f
        );
    };

}