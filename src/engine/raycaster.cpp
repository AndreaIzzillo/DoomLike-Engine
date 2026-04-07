#include "engine/raycaster.hpp"

#include "game/object.hpp"
#include "game/wall.hpp"
#include "utils/image.hpp"

#define T_MIN 1e-5f
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

            for (unsigned x = 0; x < image.getWidth(); x++)
            {
                auto ray = player.getCamera().getRay(x);
                auto record = castRay(ray, scene, T_MIN, T_MAX);

                if (record.isHit)
                {
                    auto wall = dynamic_cast<const Game::Wall *>(record.object);
                    if (wall)
                    {
                        float de = player.getCamera().getFocalDistance();
                        float hm = wall->getHeight();
                        float dm =
                            (record.point - player.getCamera().getPosition())
                                .norm();
                        float he = (de * hm) / dm;
                        float hr = static_cast<float>(image.getHeight()) / 2.0f;

                        for (unsigned y = 0; y < image.getHeight(); y++)
                        {
                            if (y > hr - he / 2 && y < hr + he / 2)
                            {
                                image(x, y) = Utils::Color(1.0f, 0.3f, 0.3f);
                            }
                        }
                    }
                }
            }
        }
    }
} // namespace Engine