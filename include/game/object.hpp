#pragma once

#include "math/math.hpp"

namespace Game
{
    struct HitRecord
    {
        Math::Point2 point;
        Math::Vector2 normal;
        float t;
        bool isHit;
    };

    class IObject
    {
    public:
        virtual ~IObject() = default;

        virtual void update(float dt) = 0;

        virtual HitRecord hit(const Math::Ray &ray, float tMin,
                              float tMax) const = 0;
    };
} // namespace Game
