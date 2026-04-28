#pragma once

#include <iostream>

namespace Math
{
    class Vector2
    {
    public:
        float x;
        float y;

        Vector2();
        Vector2(float x, float y);

        Vector2 operator+(const Vector2 &other) const;
        Vector2 operator-(const Vector2 &other) const;
        Vector2 operator*(float scalar) const;
        Vector2 operator/(float scalar) const;

        float operator*(const Vector2 &other) const;
        float operator^(const Vector2 &other) const;

        void operator+=(const Vector2 &other);
        void operator-=(const Vector2 &other);
        void operator*=(float scalar);
        void operator/=(float scalar);

        float norm() const;
        Vector2 normalized() const;

        friend std::ostream &operator<<(std::ostream &out, Vector2 &vec);
    };

} // namespace Math
