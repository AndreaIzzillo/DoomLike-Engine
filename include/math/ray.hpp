#pragma once

#include "point2.hpp"
#include "vector2.hpp"

namespace Math
{
    /**
     * @brief Normalized 2D ray used for visibility and collision queries.
     *
     * A ray carries an origin and direction and can evaluate points along its
     * parametric distance for wall intersection tests.
     */
    class Ray
    {
    public:
        Point2 origin;
        Vector2 direction;

        Ray();
        Ray(const Point2 &origin, const Vector2 &direction);

        Point2 at(float t) const;
    };
} // namespace Math
