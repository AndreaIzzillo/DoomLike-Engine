#pragma once

#include "math/point2.hpp"
#include "math/vector2.hpp"

namespace Math
{
    class Ray
    {
    public:
        Point2 origin;
        Vector2 direction;

        Ray() = default;
        Ray(const Point2 &origin, const Vector2 &direction)
            : origin(origin)
            , direction(direction.normalized())
        {}

        Point2 at(double t) const
        {
            return origin + direction * t;
        }
    };
} // namespace Math