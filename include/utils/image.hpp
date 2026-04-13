#pragma once

#include <iosfwd>
#include <vector>

namespace Utils
{
    /**
     * @brief RGB color in normalized float space.
     *
     * Supports basic arithmetic and clamping for material sampling and final
     * framebuffer conversion.
     */
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

    /**
     * @brief CPU-side 2D pixel buffer used by rendering and texture loading.
     *
     * Renderer writes into this buffer before uploading to SFML, while texture
     * materials read from file-backed images.
     */
    class Image
    {
    public:
        Image(int width, int height);
        Image(const std::string &filename);

        int getWidth() const;
        int getHeight() const;

        Color operator()(int x, int y) const;
        Color &operator()(int x, int y);

        void clear(const Color &color = Color(0, 0, 0));

    private:
        int width;
        int height;

        std::vector<Color> pixels;
    };
} // namespace Utils
