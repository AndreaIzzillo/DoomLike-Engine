#include <algorithm>
#include <cfloat>
#include <cmath>

#include "game/walls/plain_wall.hpp"

namespace Game
{
    PlainWall::PlainWall(const Math::Point2 &start, const Math::Point2 &end)
    {
        this->start = start;
        this->end = end;
    }

    PlainWall::PlainWall(const Math::Point2 &start, const Math::Point2 &end,
                         float textureScaleX, float textureOffsetX,
                         float textureScaleY, float textureOffsetY)
        : PlainWall(start, end)
    {
        this->textureScaleX = textureScaleX;
        this->textureOffsetX = textureOffsetX;
        this->textureScaleY = textureScaleY;
        this->textureOffsetY = textureOffsetY;
    }

    void PlainWall::update(float dt)
    {}

    void PlainWall::fixedUpdate(float dt)
    {}

    HitRecord PlainWall::hit(const Math::Ray &ray, float tMin, float tMax) const
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

        /* HitRecord settings */
        HitRecord rec;
        rec.isHit = true;
        /* Hit point information */
        rec.t = t;
        rec.point = ray.at(t);
        /* Texture mapping information */
        rec.u = u;
        rec.textureOffsetX = textureOffsetX;
        rec.textureScaleX = textureScaleX;
        rec.textureOffsetY = textureOffsetY;
        rec.textureScaleY = textureScaleY;
        /* Object information */
        rec.wall = this;
        rec.material = material.get();

        Math::Vector2 segDir = s.normalized();
        rec.normal = Math::Vector2(-segDir.y, segDir.x);

        return rec;
    }

    Math::Point2 PlainWall::closestPoint(const Math::Point2 &p) const
    {
        Math::Vector2 seg = end - start;
        Math::Vector2 toP = p - start;
        float t = (toP * seg) / (seg * seg);
        t = std::clamp(t, 0.f, 1.f);
        return start + seg * t;
    }
} // namespace Game
