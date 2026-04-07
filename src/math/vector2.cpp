#include "math/vector2.hpp"

#include <math.h>
#include <ostream>

namespace Math
{
    Vector2::Vector2() = default;

    Vector2::Vector2(float x, float y)
        : x(x)
        , y(y)
    {}

    Vector2 Vector2::operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 Vector2::operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 Vector2::operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 Vector2::operator/(float scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }

    float Vector2::operator*(const Vector2 &other) const
    {
        return x * other.x + y * other.y;
    }

    void Vector2::operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
    }

    void Vector2::operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
    }

    void Vector2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    void Vector2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
    }

    float Vector2::operator^(const Vector2 &other) const
    {
        return x * other.y - y * other.x;
    }

    float Vector2::norm() const
    {
        return sqrt(x * x + y * y);
    }

    Vector2 Vector2::normalized() const
    {
        float n = norm();
        return Vector2(x / n, y / n);
    }

    std::ostream &operator<<(std::ostream &out, Vector2 &vec)
    {
        return out << "(" << vec.x << ", " << vec.y << ")";
    }
} // namespace Math
