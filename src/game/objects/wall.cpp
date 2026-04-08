#include "game/objects/wall.hpp"

#include <cfloat>

namespace Game
{
    Wall::Wall(const Math::Point2 &start, const Math::Point2 &end)
        : start(start)
        , end(end)
    {}

    void Wall::update(float dt)
    {}

    void Wall::fixedUpdate(float dt)
    {}

    HitRecord Wall::hit(const Math::Ray &ray, float tMin, float tMax) const
    {
        Math::Vector2 r = ray.direction;
        Math::Vector2 s = end - start;
        Math::Vector2 diff = start - ray.origin;
        float denom = r ^ s;

        if (std::fabs(denom) < FLT_EPSILON)
            return HitRecord{};

        float t = (diff ^ s) / denom;
        float u = (diff ^ r) / denom;

        if (t < tMin || t > tMax || u < 0.f || u > 1.f)
            return HitRecord{};

        HitRecord rec;
        rec.isHit = true;
        rec.t = t;
        rec.point = ray.at(t);
        rec.object = this;
        rec.material = material.get();

        Math::Vector2 segDir = s.normalized();
        rec.normal = Math::Vector2(-segDir.y, segDir.x);

        if (rec.normal * ray.direction > 0.f)
            rec.normal = rec.normal * -1.f;

        return rec;
    }
} // namespace Game