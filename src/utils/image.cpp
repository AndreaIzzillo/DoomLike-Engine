#include "utils/image.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <ostream>
#include <stdexcept>

namespace Utils
{
    Color::Color() = default;
    Color::Color(float r, float g, float b)
        : r(r)
        , g(g)
        , b(b)
    {}
    Color::Color(int r, int g, int b)
        : r(r / 255.0f)
        , g(g / 255.0f)
        , b(b / 255.0f)
    {}

    Color Color::operator+(const Color &other) const
    {
        return Color(r + other.r, g + other.g, b + other.b);
    }

    Color Color::operator-(const Color &other) const
    {
        return Color(r - other.r, g - other.g, b - other.b);
    }

    Color Color::operator*(float scalar) const
    {
        return Color(r * scalar, g * scalar, b * scalar);
    }

    Color Color::operator/(float scalar) const
    {
        return Color(r / scalar, g / scalar, b / scalar);
    }

    bool Color::operator==(const Color &v) const
    {
        return std::abs(r - v.r) < FLT_EPSILON
            && std::abs(b - v.b) < FLT_EPSILON
            && std::abs(g - v.g) < FLT_EPSILON;
    }

    bool Color::operator!=(const Color &v) const
    {
        return !(*this == v);
    }

    void Color::operator+=(const Color &other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
    }

    void Color::operator-=(const Color &other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
    }

    void Color::operator*=(float scalar)
    {
        r *= scalar;
        g *= scalar;
        b *= scalar;
    }

    void Color::operator/=(float scalar)
    {
        r /= scalar;
        g /= scalar;
        b /= scalar;
    }

    Color Color::clamp() const
    {
        return Color(std::clamp(r, 0.0f, 1.0f), std::clamp(g, 0.0f, 1.0f),
                     std::clamp(b, 0.0f, 1.0f));
    }

    std::ostream &operator<<(std::ostream &out, Color &color)
    {
        return out << "(" << color.r << ", " << color.g << ", " << color.b
                   << ")";
    }

    Image::Image(unsigned width, unsigned height)
        : width(width)
        , height(height)
        , pixels(width * height)
    {}

    unsigned Image::getWidth() const
    {
        return width;
    }

    unsigned Image::getHeight() const
    {
        return height;
    }

    Color Image::operator()(unsigned x, unsigned y) const
    {
        if (x >= width || y >= height)
        {
            throw std::out_of_range("Pixel coordinates are out of bounds");
        }

        return pixels[y * width + x];
    }

    Color &Image::operator()(unsigned x, unsigned y)
    {
        if (x >= width || y >= height)
        {
            throw std::out_of_range("Pixel coordinates are out of bounds");
        }

        return pixels[y * width + x];
    }
} // namespace Utils
