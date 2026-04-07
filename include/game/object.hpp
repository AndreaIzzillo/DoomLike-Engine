#pragma once

#include "math/math.hpp"

namespace Game
{
    struct hitRecord
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
        virtual hitRecord hit(const Math::Ray &ray, float t_min,
                              float t_max) const = 0;
    };
} // namespace Game
