#include "engine/raycaster.hpp"

#include "game/materials/material.hpp"
#include "game/objects/object.hpp"

namespace Engine
{
    Game::HitRecord RayCaster::castRay(const Math::Ray &ray,
                                       const Game::Scene &scene, float tMin,
                                       float tMax) const
    {
        Game::HitRecord record;

        float closest = tMax;
        for (const auto &object : scene.getObjects())
        {
            auto tempRecord = object->hit(ray, tMin, closest);

            if (tempRecord.isHit)
            {
                record = tempRecord;
                closest = tempRecord.t;
            }
        }

        return record;
    }
} // namespace Engine