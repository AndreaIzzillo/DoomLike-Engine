#pragma once

#include <iosfwd>
#include <vector>

namespace Utils
{
    class Color
    {
    public:
        float r;
        float g;
        float b;

        Color();
        Color(float r, float g, float b);
        Color(int r, int g, int b);

        Color operator+(const Color &other) const;
        Color operator-(const Color &other) const;
        Color operator*(float scalar) const;
        Color operator/(float scalar) const;

        bool operator==(const Color &v) const;
        bool operator!=(const Color &v) const;

        void operator+=(const Color &other);
        void operator-=(const Color &other);
        void operator*=(float scalar);
        void operator/=(float scalar);

        Color clamp() const;

        friend std::ostream &operator<<(std::ostream &out, Color &color);
    };

    class Image
    {
    public:
        Image(unsigned width, unsigned height);

        unsigned getWidth() const;
        unsigned getHeight() const;

        void operator()(unsigned x, unsigned y, const Color &color);
        Color operator()(unsigned x, unsigned y) const;

    private:
        unsigned width;
        unsigned height;

        std::vector<Color> pixels;
    };
} // namespace Utils
