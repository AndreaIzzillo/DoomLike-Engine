#pragma once

#include <cmath>

#include "vector2.hpp"

namespace Math
{
    /**
     * @brief 2x2 matrix utility used for 2D linear transforms.
     *
     * In this project it is primarily used to rotate camera orientation vectors
     * while also exposing common matrix algebra helpers.
     */
    class Matrix2x2
    {
    public:
        float a;
        float b;
        float c;
        float d;

        Matrix2x2();
        Matrix2x2(float a, float b, float c, float d);
        Matrix2x2(float angle);

        Matrix2x2 operator+(const Matrix2x2 &other) const;
        Matrix2x2 operator-(const Matrix2x2 &other) const;
        Matrix2x2 operator*(float scalar) const;
        Matrix2x2 operator/(float scalar) const;
        Matrix2x2 operator*(const Matrix2x2 &other) const;
        Vector2 operator*(const Vector2 &vec) const;

        void operator+=(const Matrix2x2 &other);
        void operator-=(const Matrix2x2 &other);
        void operator*=(float scalar);
        void operator/=(float scalar);
        void operator*=(const Matrix2x2 &other);

        Matrix2x2 transposed() const;
        float determinant() const;
        Matrix2x2 inverse() const;

        friend std::ostream &operator<<(std::ostream &out, const Matrix2x2 &m);
    };

} // namespace Math
