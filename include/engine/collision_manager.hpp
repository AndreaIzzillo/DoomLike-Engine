#pragma once

#include "game/scene/scene.hpp"

namespace Engine
{

    /**
     * @brief Represents the resolved motion after collision handling.
     */
    struct ResolvedMotion
    {
        Math::Vector2 velocity;
        std::vector<Math::Vector2> hitNormals;
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

        ResolvedMotion resolveVelocity(Math::Vector2 intent,
                                       const Game::Player &player,
                                       const Game::Scene &scene) const;
        bool pushOut(Game::Player &player, const Game::Scene &scene) const;

    private:
        /* Hitbox size */
        static constexpr float hitboxSize = 0.25f;
    };
} // namespace Engine
