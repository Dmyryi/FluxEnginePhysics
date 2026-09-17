#pragma once

#include <cstdint>
#include <string>

namespace Flux::Domain {

    class Entity {
    private:
        std::uint64_t m_id{ 0 };
        std::string m_name{ "Entity" };
        bool m_isActive{ true };
        bool m_isVisible{ true };

    public:
        Entity() = default;
        Entity(std::uint64_t id, std::string name)
            : m_id(id), m_name(std::move(name)) {
        }

        virtual ~Entity() = default;

        std::uint64_t getId() const noexcept { return m_id; }

       
        const std::string& getName() const noexcept { return m_name; }
        void setName(std::string name) { m_name = std::move(name); }

       
        bool isActive() const noexcept { return m_isActive; }
        void setActive(bool active) noexcept { m_isActive = active; }

        bool isVisible() const noexcept { return m_isVisible; }
        void setVisible(bool visible) noexcept { m_isVisible = visible; }
    };

}