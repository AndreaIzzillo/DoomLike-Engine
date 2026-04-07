#include "game/wall.hpp"

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
            record.object = this;

        return record;
    }
} // namespace Game