#pragma once

#include <iosfwd>
#include <vector>

#define RED Utils::Color(1.f, 0.f, 0.f)
#define GREEN Utils::Color(0.f, 1.f, 0.f)
#define BLUE Utils::Color(0.f, 0.f, 1.f)
#define CYAN Utils::Color(0.f, 1.f, 1.f)
#define MAGENTA Utils::Color(1.f, 0.f, 1.f)
#define YELLOW Utils::Color(1.f, 1.f, 0.f)
#define WHITE Utils::Color(1.f, 1.f, 1.f)
#define BLACK Utils::Color(0.f, 0.f, 0.f)

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
        Color operator*(const Color &other) const;
        Color operator/(const Color &other) const;
        Color operator*(float scalar) const;
        Color operator/(float scalar) const;

        bool operator==(const Color &v) const;
        bool operator!=(const Color &v) const;

        void operator+=(const Color &other);
        void operator-=(const Color &other);
        void operator*=(const Color &other);
        void operator/=(const Color &other);
        void operator*=(float scalar);
        void operator/=(float scalar);

        Color clamp() const;
        Color clamp(float min, float max) const;

        friend std::ostream &operator<<(std::ostream &out, Color &color);
    };

    /**
     * @brief CPU-side 2D pixel buffer used by rendering and texture loading.
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

    /**
     * @brief Represents a single frame in an animated image.
     */
    struct ImageFrame
    {
        int imageIndex = 0;
        float durationMs = 0;
    };

    /**
     * @brief Manages the timeline of an animated image.
     */
    struct Scheduler
    {
        int currentIndex = 0;
        float currentDurationMs = 0;
        std::vector<ImageFrame> imageFrames = {};
    };

    /**
     * @brief A sequence of images played in succession.
     */
    class AnimatedImage
    {
    public:
        AnimatedImage(int width, int height);

        int getWidth() const;
        int getHeight() const;

        Color operator()(int x, int y) const;
        Color &operator()(int x, int y);

        void addImageFrame(const Image &image, float durationMs);
        void updateScheduler(float dt);

    private:
        int width;
        int height;

        std::vector<Image> images;
        Scheduler scheduler;
    };
} // namespace Utils
