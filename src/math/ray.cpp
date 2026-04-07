#include "math/ray.hpp"

namespace Math
{
    Ray::Ray() = default;

    Ray::Ray(const Point2 &origin, const Vector2 &direction)
        : origin(origin)
        , direction(direction.normalized())
    {}

    Point2 Ray::at(float t) const
    {
        return origin + direction * t;
    }
} // namespace Math
