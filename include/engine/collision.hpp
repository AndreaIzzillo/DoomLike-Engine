#pragma once

#include "game/scene/scene.hpp"

namespace Engine
{
    /**
     * @brief Handles collision detection between the player and the game
     * objects.
     *
     * This class is used to check if the player is colliding with any of the
     * objects in the scene, and to prevent the player from moving through them.
     */
    class CollisionManager
    {
    public:
        CollisionManager();

        Math::Vector2 resolve(Math::Vector2 intent, const Game::Player &player,
                              const Game::Scene &scene) const;

    private:
        /* Hitbox size */
        static constexpr float hitboxSize = 0.2f;
    };
} // namespace Engine