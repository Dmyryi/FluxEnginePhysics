#pragma once

namespace Flux::Domain {

    struct Vector2D {
        float x{ 0.0f };
        float y{ 0.0f };

        constexpr Vector2D() noexcept = default;
        constexpr Vector2D(float xVal, float yVal) noexcept : x(xVal), y(yVal) {}

        // Базовая арифметика
        constexpr Vector2D operator+(const Vector2D& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
        constexpr Vector2D operator-(const Vector2D& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
        constexpr Vector2D operator*(float scalar) const noexcept { return { x * scalar, y * scalar }; }
        constexpr Vector2D operator/(float scalar) const noexcept { return { x / scalar, y / scalar }; }
        constexpr Vector2D operator-() const noexcept { return { -x, -y }; }

        constexpr Vector2D& operator+=(const Vector2D& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
        constexpr Vector2D& operator-=(const Vector2D& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
        constexpr Vector2D& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; return *this; }
        constexpr Vector2D& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; return *this; }

        constexpr bool operator==(const Vector2D& rhs) const noexcept = default;

        // Векторные операции
        [[nodiscard]] constexpr float lengthSquared() const noexcept { return x * x + y * y; }
        [[nodiscard]] constexpr float dot(const Vector2D& rhs) const noexcept { return x * rhs.x + y * rhs.y; }
        [[nodiscard]] constexpr float cross(const Vector2D& rhs) const noexcept { return x * rhs.y - y * rhs.x; }
        [[nodiscard]] constexpr float distanceSquaredTo(const Vector2D& rhs) const noexcept { return (*this - rhs).lengthSquared(); }

        // Операции с вычислениями
        [[nodiscard]] float length() const noexcept;
        [[nodiscard]] Vector2D normalized() const noexcept;
        Vector2D& normalize() noexcept;
        [[nodiscard]] float distanceTo(const Vector2D& rhs) const noexcept;
        [[nodiscard]] Vector2D rotated(float angleRadians) const noexcept;
    };

    constexpr Vector2D operator*(float scalar, const Vector2D& vec) noexcept {
        return vec * scalar;
    }

}