#include "game/objects/wall.hpp"

namespace Game
{
    Wall::Wall(const Segment &segment, float height)
        : segment(segment)
        , height(height)
    {}

    Wall::Wall(const Math::Point2 &start, const Math::Point2 &end, float height)
        : segment(start, end)
        , height(height)
    {}

    float Wall::getHeight() const
    {
        return height;
    }

    void Wall::update(float dt)
    {}

    void Wall::fixedUpdate(float dt)
    {}

    HitRecord Wall::hit(const Math::Ray &ray, float tMin, float tMax) const
    {
        auto record = segment.hit(ray, tMin, tMax);
        if (record.isHit)
        {
            record.object = this;
            record.material = material.get();
        }

        return record;
    }

    void Wall::extrude(const Math::Ray &ray, const HitRecord &record,
                       const Player &player, Utils::Image &image,
                       unsigned x) const
    {
        float de = player.getCamera().getFocalDistance();
        float hm = getHeight();
        float dm = record.t * (ray.direction * player.getCamera().getForward());
        float he = (de * hm) / dm;
        float hr = static_cast<float>(image.getHeight()) / 2.0f;

        for (unsigned y = 0; y < image.getHeight(); y++)
        {
            if (y > hr - he / 2 && y < hr + he / 2)
            {
                auto properties = record.material->getProperties(record);
                image(x, y) = properties.color;
            }
        }
    }
} // namespace Game