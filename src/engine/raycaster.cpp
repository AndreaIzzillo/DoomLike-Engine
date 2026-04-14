#include "engine/raycaster.hpp"

#include <cfloat>

#include "game/world/wall.hpp"

namespace Engine
{
    std::vector<Game::HitRecord> RayCaster::castRay(const Math::Ray &ray, const Game::Scene &scene,
                                                    float tMin, float tMax) const
    {
        std::vector<Game::HitRecord> hitRecords;

        const Game::Sector *currentSector = scene.getCurrentSector();

        float from = tMin;
        while (currentSector)
        {
            Game::HitRecord record;

            /* Find the closest wall in the current sector */
            float closest = tMax;
            for (const auto &wall : currentSector->getWalls())
            {
                auto tempRecord = wall->hit(ray, from, closest);
                if (tempRecord.isHit)
                {
                    closest = tempRecord.t;
                    record = tempRecord;
                }
            }

            if (record.isHit)
            {
                hitRecords.push_back(record);
                currentSector =
                    currentSector == record.frontSector ? record.backSector : record.frontSector;
                from = record.t + 1e-5f;
            }
            else
            {
                currentSector = nullptr;
            }
        }

        return hitRecords;
    }
} // namespace Engine
