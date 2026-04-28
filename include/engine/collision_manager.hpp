#pragma once

#include "game/scene/scene.hpp"

namespace Engine
{

    struct CollisionInfo
    {
        Math::Vector2 velocity = { 0.f, 0.f };
        const Game::Sector *newSector = nullptr;
    };

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
