#pragma once

#include <iostream>

#include "vector2.hpp"

namespace Math
{
    class Point2
    {
    public:
        float x;
        float y;

        Point2() = default;
        Point2(float x, float y)
            : x(x)
            , y(y)
        {}

        Point2 operator+(const Point2 &other) const
        {
            return Point2(x + other.x, y + other.y);
        }

        Point2 operator+(const Vector2 &other) const
        {
            return Point2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Point2 &other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        Point2 operator-(const Vector2 &other) const
        {
            return Point2(x - other.x, y - other.y);
        }

        Point2 operator*(float scalar) const
        {
            return Point2(x * scalar, y * scalar);
        }

        Point2 operator/(float scalar) const
        {
            return Point2(x / scalar, y / scalar);
        }

        friend std::ostream &operator<<(std::ostream &out, Point2 &point)
        {
            return out << "(" << point.x << ", " << point.y << ")";
        }
    };

} // namespace Math