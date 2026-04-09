#include "engine/collision_manager.hpp"

#include <cfloat>

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    Math::Vector2
    CollisionManager::resolveVelocity(Math::Vector2 intent,
                                      const Game::Player &player,
                                      const Game::Scene &scene) const
    {
        const auto &camera = player.getCamera();
        const Math::Vector2 relativeIntent =
            camera.getRight() * intent.x + camera.getForward() * intent.y;
        const float norm = relativeIntent.norm();

        if (norm < FLT_EPSILON)
            return intent;

        for (auto &object : scene.getObjects())
        {
            /* Collision is checked using the hit function of the walls */
            Game::HitRecord hit =
                object->hit(Math::Ray(player.getPosition(), relativeIntent),
                            0.f, norm + hitboxSize);

            /* If a collision is detected, we return a zero velocity to stop the
             * player */
            if (hit.isHit)
            {
                return Math::Vector2(0.f, 0.f);
            }
        }

        /* If no collision is detected, we return the original intent */
        return intent;
    }
} // namespace Engine
