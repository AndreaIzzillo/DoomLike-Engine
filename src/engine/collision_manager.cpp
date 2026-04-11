#include "engine/collision_manager.hpp"

#include <cfloat>

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    ResolvedMotion
    CollisionManager::resolveVelocity(Math::Vector2 intent,
                                      const Game::Player &player,
                                      const Game::Scene &scene) const
    {
        const auto &camera = player.getCamera();
        Math::Vector2 relativeIntent =
            camera.getRight() * intent.x + camera.getForward() * intent.y;
        const float norm = relativeIntent.norm();

        if (norm < FLT_EPSILON)
            return { intent, {} };

        std::vector<Math::Vector2> hitNormals;

        for (auto &wall : scene.getWalls())
        {
            Game::HitRecord hit =
                wall->hit(Math::Ray(player.getPosition(), relativeIntent),
                          FLT_EPSILON, norm + hitboxSize);

            if (hit.isHit)
            {
                // We need all normals that are colliding to resolve the velocity correctly in corners
                hitNormals.push_back(hit.normal );
            }
        }

        Math::Vector2 resolved = relativeIntent;
        for (const auto &normal : hitNormals)
        {
            float penetration = resolved * normal;
            if (penetration < 0.f)
                resolved = resolved - normal * penetration;
        }

        const Math::Vector2 right = camera.getRight();
        const Math::Vector2 forward = camera.getForward();
        return { Math::Vector2(resolved * right, resolved * forward),
                 hitNormals };
    }

    bool CollisionManager::pushOut(Game::Player &player,
                                   const Game::Scene &scene) const
    {
        bool isPushed = false;
        for (auto &wall : scene.getWalls())
        {
            Math::Point2 closest = wall->closestPoint(player.getPosition());
            Math::Vector2 delta = player.getPosition() - closest;
            float dist = delta.norm();

            /* The player is too close to the wall */
            if (dist < hitboxSize && dist > FLT_EPSILON)
            {
                Math::Vector2 pushDir = delta / dist;
                player.nudge(pushDir * (hitboxSize - dist));
                isPushed = true;
            }
            /* Degenerate case: player exactly on the segment */
            else if (dist <= FLT_EPSILON)
            {
                Math::Vector2 seg =
                    (wall->getEnd() - wall->getStart()).normalized();
                Math::Vector2 normal(-seg.y, seg.x);
                player.nudge(normal * hitboxSize);
                isPushed = true;
            }
        }
        return isPushed;
    }

} // namespace Engine
