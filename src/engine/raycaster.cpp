#include "engine/raycaster.hpp"

#include <cfloat>

#include "game/materials/material.hpp"
#include "game/objects/object.hpp"
#include "utils/image.hpp"

#define T_MIN FLT_EPSILON
#define T_MAX std::numeric_limits<float>::infinity()

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

    void RayCaster::renderFrame(const Game::Scene &scene,
                                Utils::Image &image) const
    {
        const auto &player = scene.getPlayer();

        for (unsigned y = 0; y < image.getHeight(); y++)
        {
            for (unsigned x = 0; x < image.getWidth(); x++)
            {
                if (y < image.getHeight() / 2)
                {
                    image(x, y) = Utils::Color(0.5f, 0.7f, 1.0f);
                }
                else
                {
                    image(x, y) = Utils::Color(0.5f, 0.5f, 0.5f);
                }
            }
        }

        for (unsigned x = 0; x < image.getWidth(); x++)
        {
            auto ray = player.getCamera().getRay(x);
            auto record = castRay(ray, scene, T_MIN, T_MAX);

            if (record.isHit)
            {
                auto hitObject = record.object;
                hitObject->extrude(ray, record, player, image, x);
            }
        }
    }
} // namespace Engine