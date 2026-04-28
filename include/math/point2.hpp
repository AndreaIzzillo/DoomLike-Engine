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

        Point2();
        Point2(float x, float y);

        Point2 operator+(const Point2 &other) const;
        Point2 operator+(const Vector2 &other) const;
        Vector2 operator-(const Point2 &other) const;
        Point2 operator-(const Vector2 &other) const;
        Point2 operator*(float scalar) const;
        Point2 operator/(float scalar) const;

        void operator+=(const Point2 &other);
        void operator+=(const Vector2 &other);
        void operator-=(const Point2 &other);
        void operator-=(const Vector2 &other);
        void operator*=(float scalar);
        void operator/=(float scalar);

        friend std::ostream &operator<<(std::ostream &out, Point2 &point);
    };

} // namespace Math
