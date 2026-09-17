// domain/include/domain/PhysicsBody.h
#pragma once

#include "domain/Entity.h"
#include "domain/Vector2D.h"
#include <vector>
#include <cstdint>
#include <string>

namespace Flux::Domain {

    class IBodyVisitor;
    struct ChargeElement;

    class PhysicsBody : public Entity {
    private:
        Vector2D m_position{ 0.0f, 0.0f };
        Vector2D m_velocity{ 0.0f, 0.0f };
        Vector2D m_force{ 0.0f, 0.0f };
        float m_mass{ 1.0f };
        bool m_isPinned{ false };

    public:
        PhysicsBody() = default;
        PhysicsBody(std::uint64_t id, std::string name, const Vector2D& pos, float mass = 1.0f)
            : Entity(id, std::move(name)), m_position(pos) {
            setMass(mass);
        }

        virtual ~PhysicsBody() override = default;
        virtual void accept(IBodyVisitor& visitor) const = 0;

        virtual void accept(IBodyVisitor& visitor) = 0;
        virtual std::vector<ChargeElement> getChargeElements() const = 0;

        // --- Интегратор физики ---
        void integrate(float dt) noexcept {
            // Если тело зафиксировано гвоздем или масса нулевая — не двигаем
            if (m_isPinned || m_mass <= 0.0001f) {
                clearForce();
                return;
            }

            // a = F / m
            Vector2D acceleration = m_force / m_mass;

            // v = v + a * dt
            m_velocity += acceleration * dt;

            // x = x + v * dt
            m_position += m_velocity * dt;

            // Сбрасываем силы в конце каждого шага, чтобы они не копились вечно
            clearForce();
        }

        // Позиция
        const Vector2D& getPosition() const noexcept { return m_position; }
        void setPosition(const Vector2D& pos) noexcept { m_position = pos; }

        // Скорость
        const Vector2D& getVelocity() const noexcept { return m_velocity; }
        void setVelocity(const Vector2D& vel) noexcept { m_velocity = vel; }

        // Сила
        const Vector2D& getForce() const noexcept { return m_force; }
        void applyForce(const Vector2D& f) noexcept { m_force += f; }
        void clearForce() noexcept { m_force = { 0.0f, 0.0f }; }

        // Масса
        float getMass() const noexcept { return m_mass; }
        void setMass(float mass) noexcept {
            if (mass > 0.0001f) {
                m_mass = mass;
            }
        }

        // Фиксация тела на месте
        bool isPinned() const noexcept { return m_isPinned; }
        void setPinned(bool pinned) noexcept { m_isPinned = pinned; }
    };

}