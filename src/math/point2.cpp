#include "math/point2.hpp"

#include <ostream>

namespace Math
{
    Point2::Point2() = default;

    Point2::Point2(float x, float y)
        : x(x)
        , y(y)
    {}

    Point2 Point2::operator+(const Point2 &other) const
    {
        return Point2(x + other.x, y + other.y);
    }

    Point2 Point2::operator+(const Vector2 &other) const
    {
        return Point2(x + other.x, y + other.y);
    }

    Vector2 Point2::operator-(const Point2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Point2 Point2::operator-(const Vector2 &other) const
    {
        return Point2(x - other.x, y - other.y);
    }

    Point2 Point2::operator*(float scalar) const
    {
        return Point2(x * scalar, y * scalar);
    }

    Point2 Point2::operator/(float scalar) const
    {
        return Point2(x / scalar, y / scalar);
    }

    void Point2::operator+=(const Point2 &other)
    {
        x += other.x;
        y += other.y;
    }

    void Point2::operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
    }

    void Point2::operator-=(const Point2 &other)
    {
        x -= other.x;
        y -= other.y;
    }

    void Point2::operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
    }

    void Point2::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    void Point2::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
    }

    std::ostream &operator<<(std::ostream &out, Point2 &point)
    {
        return out << "(" << point.x << ", " << point.y << ")";
    }
} // namespace Math
