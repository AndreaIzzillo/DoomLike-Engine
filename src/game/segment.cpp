#pragma once

#include "segment.hpp"

#include "object.hpp"

namespace Game
{
    Segment::Segment(const Math::Point2 &start, const Math::Point2 &end)
    {
        start_ = start;
        end_ = end;
    }

    void Segment::update(float dt)
    {}

    hitRecord Segment::hit(const Math::Ray &ray, float t_min, float t_max) const
    {
        Math::Vector2 r = ray.direction;
        Math::Vector2 s = end_ - start_;
        Math::Vector2 diff = start_ - ray.origin;

        float denom = r ^ s;

        if (std::fabs(denom) < FLT_EPSILON)
            return hitRecord{};

        float t = diff ^ s / denom;
        float u = r ^ diff / denom;

        if (t < t_min || t > t_max || u < 0.f || u > 1.f)
            return hitRecord{};

        hitRecord rec;
        rec.t = t;
        rec.point = ray.origin + ray.direction * t;

        Math::Vector2 segDir = s.normalized();
        rec.normal = Math::Vector2(-segDir.y, segDir.x);

        if (rec.normal * ray.direction > 0.f)
            rec.normal = rec.normal * -1.f;

        return rec;
    }
} // namespace Game
