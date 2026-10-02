#pragma once

#include "domain/Vector2D.h"
#include "domain/ChargeElement.h"
#include <vector>
#include <cmath>

namespace Flux::Domain {

    class PhysicSolver {
    public:
       
        static Vector2D calculateFieldAtPoint(
            const Vector2D& point,
            const ChargeElement& source,
            float k = 8.98755e9f,
            float minDistance = 5.0f
        ) noexcept {
            Vector2D diff = point - source.position;
            
            float distSq = diff.lengthSquared();

            if (distSq < minDistance * minDistance) {
                distSq = minDistance * minDistance;
            }

            float dist = std::sqrt(distSq);
            Vector2D dir = diff / dist;

            // E = k * q / r^2
            float magnitude = (k * source.charge) / distSq;
            return dir * magnitude;
        }

        static Vector2D calculateForceBetweenBodies(
            const std::vector<ChargeElement>& sourceElements,
            const std::vector<ChargeElement>& targetElements,
            float k = 8.98755e9f,
            float minDistance = 5.0f
        ) noexcept {
            Vector2D totalForce{ 0.0f, 0.0f };

            for (const auto& targetElem : targetElements) {
                Vector2D fieldAtTarget{ 0.0f, 0.0f };

                for (const auto& srcElem : sourceElements) {
                    fieldAtTarget += calculateFieldAtPoint(targetElem.position, srcElem, k, minDistance);
                }

                // F = q * E
                totalForce += fieldAtTarget * targetElem.charge;
            }

            return totalForce;
        }

       
        static Vector2D calculateAirResistance(const Vector2D& velocity, float dragCoeff) noexcept {
            return velocity * (-dragCoeff);
        }
    };

}