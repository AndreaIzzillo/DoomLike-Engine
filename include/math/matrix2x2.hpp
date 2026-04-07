#pragma once

#include <math.h>

#include "vector2.hpp"

namespace Math
{
    class Matrix2x2
    {
    public:
        float a;
        float b;
        float c;
        float d;

        Matrix2x2() = default;
        Matrix2x2(float a, float b, float c, float d)
            : a(a)
            , b(b)
            , c(c)
            , d(d)
        {}
        Matrix2x2(float angle)
        {
            float cosA = cos(angle);
            float sinA = sin(angle);
            a = cosA;
            b = -sinA;
            c = sinA;
            d = cosA;
        }

        Matrix2x2 operator+(const Matrix2x2 &other) const
        {
            return Matrix2x2(a + other.a, b + other.b, c + other.c,
                             d + other.d);
        }

        Matrix2x2 operator-(const Matrix2x2 &other) const
        {
            return Matrix2x2(a - other.a, b - other.b, c - other.c,
                             d - other.d);
        }

        Matrix2x2 operator*(float scalar) const
        {
            return Matrix2x2(a * scalar, b * scalar, c * scalar, d * scalar);
        }

        Matrix2x2 operator/(float scalar) const
        {
            return Matrix2x2(a / scalar, b / scalar, c / scalar, d / scalar);
        }

        Matrix2x2 operator*(const Matrix2x2 &other) const
        {
            return Matrix2x2(
                a * other.a + b * other.c, a * other.b + b * other.d,
                c * other.a + d * other.c, c * other.b + d * other.d);
        }

        Vector2 operator*(const Vector2 &vec) const
        {
            return Vector2(a * vec.x + b * vec.y, c * vec.x + d * vec.y);
        }

        Matrix2x2 transposed() const
        {
            return Matrix2x2(a, c, b, d);
        }

        float determinant() const
        {
            return a * d - b * c;
        }

        Matrix2x2 inverse() const
        {
            float det = determinant();
            return Matrix2x2(d / det, -b / det, -c / det, a / det);
        }
    };

} // namespace Math