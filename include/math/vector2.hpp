#pragma once

#include <iostream>
#include <math.h>

namespace Math
{
    class Vector2
    {
    public:
        float x;
        float y;

        Vector2() = default;
        Vector2(float x, float y)
            : x(x)
            , y(y)
        {}

        Vector2 operator+(const Vector2 &other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Vector2 &other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator*(float scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        Vector2 operator/(float scalar) const
        {
            return Vector2(x / scalar, y / scalar);
        }

        float operator*(const Vector2 &other) const
        {
            return x * other.x + y * other.y;
        }

        void operator+=(const Vector2 &other)
        {
            x += other.x;
            y += other.y;
        }

        void operator-=(const Vector2 &other)
        {
            x -= other.x;
            y -= other.y;
        }

        void operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
        }

        void operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
        }

        float norm() const
        {
            return sqrt(x * x + y * y);
        }

        Vector2 normalized() const
        {
            float n = norm();
            return Vector2(x / n, y / n);
        }

        friend std::ostream &operator<<(std::ostream &out, Vector2 &vec)
        {
            return out << "(" << vec.x << ", " << vec.y << ")";
        }
    };

} // namespace Math