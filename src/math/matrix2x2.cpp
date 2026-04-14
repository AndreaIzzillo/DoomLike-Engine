#include "math/matrix2x2.hpp"

#include <iomanip>
#include <ostream>

namespace Math
{
    Matrix2x2::Matrix2x2() = default;

    Matrix2x2::Matrix2x2(float a, float b, float c, float d)
        : a(a)
        , b(b)
        , c(c)
        , d(d)
    {}

    Matrix2x2::Matrix2x2(float angle)
    {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        a = cosA;
        b = -sinA;
        c = sinA;
        d = cosA;
    }

    Matrix2x2 Matrix2x2::operator+(const Matrix2x2 &other) const
    {
        return Matrix2x2(a + other.a, b + other.b, c + other.c, d + other.d);
    }

    Matrix2x2 Matrix2x2::operator-(const Matrix2x2 &other) const
    {
        return Matrix2x2(a - other.a, b - other.b, c - other.c, d - other.d);
    }

    Matrix2x2 Matrix2x2::operator*(float scalar) const
    {
        return Matrix2x2(a * scalar, b * scalar, c * scalar, d * scalar);
    }

    Matrix2x2 Matrix2x2::operator/(float scalar) const
    {
        return Matrix2x2(a / scalar, b / scalar, c / scalar, d / scalar);
    }

    Matrix2x2 Matrix2x2::operator*(const Matrix2x2 &other) const
    {
        return Matrix2x2(a * other.a + b * other.c, a * other.b + b * other.d,
                         c * other.a + d * other.c, c * other.b + d * other.d);
    }

    Vector2 Matrix2x2::operator*(const Vector2 &vec) const
    {
        return Vector2(a * vec.x + b * vec.y, c * vec.x + d * vec.y);
    }

    void Matrix2x2::operator+=(const Matrix2x2 &other)
    {
        a += other.a;
        b += other.b;
        c += other.c;
        d += other.d;
    }

    void Matrix2x2::operator-=(const Matrix2x2 &other)
    {
        a -= other.a;
        b -= other.b;
        c -= other.c;
        d -= other.d;
    }

    void Matrix2x2::operator*=(float scalar)
    {
        a *= scalar;
        b *= scalar;
        c *= scalar;
        d *= scalar;
    }

    void Matrix2x2::operator/=(float scalar)
    {
        a /= scalar;
        b /= scalar;
        c /= scalar;
        d /= scalar;
    }

    void Matrix2x2::operator*=(const Matrix2x2 &other)
    {
        *this = *this * other;
    }

    Matrix2x2 Matrix2x2::transposed() const
    {
        return Matrix2x2(a, c, b, d);
    }

    float Matrix2x2::determinant() const
    {
        return a * d - b * c;
    }

    Matrix2x2 Matrix2x2::inverse() const
    {
        float det = determinant();
        return Matrix2x2(d / det, -b / det, -c / det, a / det);
    }

    std::ostream &operator<<(std::ostream &out, const Matrix2x2 &m)
    {
        out << std::fixed << std::setprecision(2);

        out << "┌             ┐\n";
        out << "│ " << std::setw(8) << m.a << " " << std::setw(8) << m.b << " │\n";
        out << "│ " << std::setw(8) << m.c << " " << std::setw(8) << m.d << " │\n";
        out << "└             ┘";

        return out;
    }
} // namespace Math
