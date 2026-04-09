#include "engine/collision.hpp"

#include <cfloat>

#include "math/vector2.hpp"

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    Math::Vector2 CollisionManager::resolve(Math::Vector2 intent,
                                            const Game::Player &player,
                                            const Game::Scene &scene) const
    {
        const auto &camera = player.getCamera();
        const Math::Vector2 worldIntent =
            camera.getRight() * intent.x + camera.getForward() * intent.y;
        const float norm = worldIntent.norm();

        if (norm < FLT_EPSILON)
            return intent;

        for (auto &object : scene.getObjects())
        {
            Game::HitRecord hit = object->hit(
                Math::Ray(player.getPosition(), worldIntent), 0.f,
                norm + hitboxSize); // Check for collisions along the path of
                                    // the intent, with a small margin
            if (hit.isHit)
            {
                return Math::Vector2(0.f,
                                     0.f); // Collision detected, stop movement
            }
        }

        return intent; // No collision, return original intent
    }
} // namespace Engine
