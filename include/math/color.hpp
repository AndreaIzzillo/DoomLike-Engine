#pragma once

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <math.h>

namespace Math
{
    class Color
    {
    public:
        float r;
        float g;
        float b;

        Color() = default;
        Color(float r, float g, float b)
            : r(r)
            , g(g)
            , b(b)
        {}

        Color operator+(const Color &other) const
        {
            return Color(r + other.r, g + other.g, b + other.b);
        }

        Color operator-(const Color &other) const
        {
            return Color(r - other.r, g - other.g, b - other.b);
        }

        Color operator*(float scalar) const
        {
            return Color(r * scalar, g * scalar, b * scalar);
        }

        Color operator/(float scalar) const
        {
            return Color(r / scalar, g / scalar, b / scalar);
        }

        bool operator==(const Color &v) const
        {
            return abs(r - v.r) < FLT_EPSILON && abs(b - v.b) < FLT_EPSILON
                && abs(g - v.g) < FLT_EPSILON;
        }

        bool operator!=(const Color &v) const
        {
            return !(*this == v);
        }

        Color clamp() const
        {
            return Color(std::clamp(r, 0.0f, 1.0f), std::clamp(g, 0.0f, 1.0f),
                         std::clamp(b, 0.0f, 1.0f));
        }

        friend std::ostream &operator<<(std::ostream &out, Color &color)
        {
            return out << "(" << color.r << ", " << color.g << ", " << color.b
                       << ")";
        }
    };
} // namespace Math