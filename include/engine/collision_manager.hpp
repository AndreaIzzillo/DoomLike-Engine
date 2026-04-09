#pragma once

#include "game/scene/scene.hpp"
#include "math/point2.hpp"

namespace Engine
{
    /**
     * @brief Resolves player movement intent against world geometry.
     *
     * During fixed updates it converts player-local intent to world direction,
     * tests wall intersections, and returns a blocked or unchanged velocity
     * intent.
     */
    class CollisionManager
    {
    public:
        CollisionManager();

        Math::Vector2 resolveVelocity(Math::Vector2 intent,
                                      const Game::Player &player,
                                      const Game::Scene &scene) const;
    private:
        /* Hitbox size */
        static constexpr float hitboxSize = 0.1f;
    };
} // namespace Engine
