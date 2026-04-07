#pragma once

#include "point2.hpp"
#include "vector2.hpp"

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
    };
} // namespace Math