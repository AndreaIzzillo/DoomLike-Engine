#include "engine/collision_manager.hpp"

#include <algorithm>
#include <cfloat>

#include "math/vector2.hpp"

namespace Engine
{
    CollisionManager::CollisionManager()
    {}

    CollisionInfo CollisionManager::computeCollision(Math::Vector2 intent,
                                                     const Game::Player &player,
                                                     const Game::Scene &scene,
                                                     float dt) const
    {
        (void)dt;
        const auto &camera = player.getCamera();
        const Math::Vector2 right = camera.getRight();
        const Math::Vector2 forward = camera.getForward();
        const Math::Vector2 relativeIntent =
            right * intent.x + forward * intent.y;
        const float norm = relativeIntent.norm();

        if (norm < FLT_EPSILON)
            return { intent };

        const auto &walls = scene.getCurrentSector()->getWalls();
        const auto *currentSector = scene.getCurrentSector();
        const Math::Ray intentRay(player.getPosition(), relativeIntent);
        const float requiredHeight = player.getSize() + player.getUpperHitBox();

        CollisionInfo res;
        std::vector<Math::Vector2> hitNormals;
        std::vector<const Game::Wall *> passablePortals;
        float traversedPortalT = FLT_MAX;

        // Pass 1: gather blocking normals and detect passable portal.
        for (const auto &wall : walls)
        {
            Game::HitRecord hit =
                wall->hit(intentRay, FLT_EPSILON, norm);
            if (!hit.isHit)
                continue;

            const Math::Vector2 hitNormal = (relativeIntent * hit.normal < 0.f)
                ? hit.normal
                : hit.normal * -1.f;

            if (wall->isPortal())
            {
                const bool portalAdjacentToPlayer =
                    hit.frontSector == currentSector
                    || hit.backSector == currentSector;

                if (portalAdjacentToPlayer)
                {
                    const auto *targetSector =
                        (hit.frontSector == currentSector) ? hit.backSector
                                                           : hit.frontSector;
                    const bool enoughHeadroom = (targetSector->getCeilingHeight()
                            - targetSector->getFloorHeight())
                            >= requiredHeight;
                    const bool canStep = wall->stepHeight()
                        <= player.getJumpHeight() + FLT_EPSILON;

                    if (enoughHeadroom && canStep)
                    {
                        if (std::find(passablePortals.begin(),
                                      passablePortals.end(),
                                      wall)
                            == passablePortals.end())
                            passablePortals.push_back(wall);

                        if (hit.t < traversedPortalT)
                        {
                            traversedPortalT = hit.t;
                            res.newSector = targetSector;
                        }
                        continue;
                    }
                }
            }

            // Non-passable portal or plain wall: treat as blocking.
            hitNormals.push_back(hitNormal);
        }

        // Resolve velocity against blocking normals.
        Math::Vector2 resolved = relativeIntent;
        for (const auto &normal : hitNormals)
        {
            const float pen = resolved * normal;
            if (pen < 0.f)
                resolved = resolved - normal * pen;
        }

        // Pass 2: iterative positional separation on all blocking walls from
        // the current sector.
        Math::Point2 correctedPos = player.getPosition() + resolved;
        for (int iter = 0; iter < 4; ++iter)
        {
            Math::Vector2 correction(0.f, 0.f);
            bool hadPenetration = false;

            for (const auto *wall : walls)
            {
                // not optimal but this should be enough to have good collision
                if (wall->isPortal() || std::find(passablePortals.begin(), passablePortals.end(),
                              wall)
                    != passablePortals.end())
                    continue;

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
                    const Math::Vector2 seg =
                        (wall->getEnd() - wall->getStart()).normalized();
                    pushDir = Math::Vector2(-seg.y, seg.x);
                }

                correction = correction + pushDir * (hitboxSize - dist);
                hadPenetration = true;
            }

            if (!hadPenetration)
                break;

            const float corrNorm = correction.norm();
            if (corrNorm > hitboxSize && corrNorm > FLT_EPSILON)
                correction = correction * (hitboxSize / corrNorm);

            correctedPos += correction;
        }

        // Re-projection: push must not reintroduce movement toward blocked
        // walls.
        Math::Vector2 finalWorld = correctedPos - player.getPosition();
        for (const auto &normal : hitNormals)
        {
            const float pen = finalWorld * normal;
            if (pen < 0.f)
                finalWorld = finalWorld - normal * pen;
        }

        res.velocity = Math::Vector2(finalWorld * right, finalWorld * forward);
        return res;
    }
} // namespace Engine
