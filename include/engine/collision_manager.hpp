#pragma once

#include "game/scene/scene.hpp"

namespace Engine
{

    /**
     * @brief Represents the resolved motion after collision handling.
     */
    struct CollisionInfo
    {
        Math::Vector2 velocity = { 0.f, 0.f };
        const Game::Sector *newSector = nullptr;
    };

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

        CollisionInfo computeCollision(Math::Vector2 intent, const Game::Player &player,
                                       const Game::Scene &scene) const;

    private:
        /* Hitbox size */
        static constexpr float hitboxSize = 0.2f;
    };
} // namespace Engine
