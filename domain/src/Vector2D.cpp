#include "domain/Vector2D.h"
#include <cmath>

namespace Flux::Domain {

    float Vector2D::length() const noexcept {
        return std::sqrt(lengthSquared());
    }

    Vector2D Vector2D::normalized() const noexcept {
        const float len = length();
        if (len > 0.0f) {
            return *this / len;
        }
        return *this;
    }

    Vector2D& Vector2D::normalize() noexcept {
        *this = normalized();
        return *this;
    }

    float Vector2D::distanceTo(const Vector2D& rhs) const noexcept {
        return (*this - rhs).length();
    }

    Vector2D Vector2D::rotated(float angleRadians) const noexcept {
        const float cosA = std::cos(angleRadians);
        const float sinA = std::sin(angleRadians);
        return { x * cosA - y * sinA, x * sinA + y * cosA };
    }

}