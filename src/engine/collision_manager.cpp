#include "engine/collision_manager.hpp"

#include <cfloat>

#include "game/world/wall.hpp"
#include "math/vector2.hpp"

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    CollisionInfo CollisionManager::computeCollision(Math::Vector2 intent,
                                                     const Game::Player &player,
                                                     const Game::Scene &scene) const
    {
        const auto &camera = player.getCamera();
        const Math::Vector2 right = camera.getRight();
        const Math::Vector2 forward = camera.getForward();
        const Math::Vector2 relativeIntent = right * intent.x + forward * intent.y;
        const float norm = relativeIntent.norm();

        if (norm < FLT_EPSILON)
            return { intent };

        const auto &walls = scene.getCurrentSector()->getWalls();
        const auto *currentSector = scene.getCurrentSector();
        const Math::Ray intentRay(player.getPosition(), relativeIntent);
        const float requiredHeight = player.getSize() + player.getUpperHitBox();

        CollisionInfo res;

        Math::Point2 correctedPos = player.getPosition() + relativeIntent;

        for (const auto *wall : walls)
        {
            if (wall->canTraverse(requiredHeight, player.getJumpHeight(), currentSector))
            {
                continue;
            }

            const Math::Point2 closest = wall->closestPoint(correctedPos);
            const Math::Vector2 delta = correctedPos - closest;
            const float dist = delta.norm();

            if (dist >= hitboxSize - FLT_EPSILON)
                continue;

            Math::Vector2 pushDir;
            if (dist > FLT_EPSILON)
            {
                pushDir = delta / dist;
            }
            else
            {
                const Math::Vector2 seg = (wall->getEnd() - wall->getStart()).normalized();
                pushDir = Math::Vector2(-seg.y, seg.x);
            }

            correctedPos += pushDir * (hitboxSize - dist);
        }

        Math::Vector2 finalWorld = correctedPos - player.getPosition();
        res.displacement = Math::Vector2(finalWorld * right, finalWorld * forward);

        for (const auto *wall : walls)
        {
            if (wall->canTraverse(requiredHeight, player.getJumpHeight(), currentSector))
            {
                const auto hit = wall->hit(intentRay, 0.f, norm);
                if (hit.isHit)
                {
                    auto frontSector = scene.getCurrentSector();
                    auto backSector =
                        (frontSector == hit.frontSector) ? hit.backSector : hit.frontSector;

                    res.newSector = backSector;
                }
            }
        }

        return res;
    }
} // namespace Engine
