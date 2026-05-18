#include "utils/image.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

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

    Color Color::operator*(const Color &other) const
    {
        return Color(r * other.r, g * other.g, b * other.b);
    }

    Color Color::operator/(const Color &other) const
    {
        return Color(r / other.r, g / other.g, b / other.b);
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
        return std::abs(r - v.r) < FLT_EPSILON && std::abs(b - v.b) < FLT_EPSILON
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

    void Color::operator*=(const Color &other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
    }

    void Color::operator/=(const Color &other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
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

    Color Color::clamp(float min, float max) const
    {
        return Color(std::clamp(r, min, max), std::clamp(g, min, max), std::clamp(b, min, max));
    }

    std::ostream &operator<<(std::ostream &out, Color &color)
    {
        return out << "(" << color.r << ", " << color.g << ", " << color.b << ")";
    }

    Image::Image(int width, int height)
        : width(width)
        , height(height)
        , pixels(width * height)
    {}

    Image::Image(const std::string &filename)
    {
        auto file = std::ifstream(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open image file: " + filename);
        }

        auto next_token = [&](std::istream &s) -> std::string {
            std::string t;
            while (s >> t)
            {
                if (!t.empty() && t[0] == '#')
                {
                    std::string rest;
                    std::getline(s, rest);
                    continue;
                }
                return t;
            }
            return std::string();
        };

        std::string magic = next_token(file);
        if (magic != "P3")
        {
            throw std::runtime_error("Unsupported PPM format (only P3 supported): " + magic);
        }

        std::string wtok = next_token(file);
        std::string htok = next_token(file);
        std::string mvtok = next_token(file);
        if (wtok.empty() || htok.empty() || mvtok.empty())
        {
            throw std::runtime_error("Invalid PPM header in file: " + filename);
        }

        width = std::stoi(wtok);
        height = std::stoi(htok);
        const int maxval = std::stoi(mvtok);

        pixels = std::vector<Color>(width * height);

        int i = 0;
        while (i < width * height)
        {
            std::string rs = next_token(file);
            std::string gs = next_token(file);
            std::string bs = next_token(file);
            if (rs.empty() || gs.empty() || bs.empty())
            {
                break;
            }
            int ri = std::stoi(rs);
            int gi = std::stoi(gs);
            int bi = std::stoi(bs);
            float denom = maxval > 0 ? static_cast<float>(maxval) : 255.f;
            pixels[i].r = ri / denom;
            pixels[i].g = gi / denom;
            pixels[i].b = bi / denom;
            ++i;
        }

        if (i != width * height)
        {
            throw std::runtime_error("PPM pixel count does not match header in file: " + filename);
        }
    }

    int Image::getWidth() const
    {
        return width;
    }

    int Image::getHeight() const
    {
        return height;
    }

    Color Image::operator()(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            throw std::out_of_range("Pixel coordinates are out of bounds");
        }

        return pixels[y * width + x];
    }

    Color &Image::operator()(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            throw std::out_of_range("Pixel coordinates are out of bounds");
        }

        return pixels[y * width + x];
    }

    void Image::clear(const Color &color)
    {
        std::fill(pixels.begin(), pixels.end(), color);
    }

    AnimatedImage::AnimatedImage(int width, int height)
        : width(width)
        , height(height)
    {}

    int AnimatedImage::getWidth() const
    {
        return width;
    }

    int AnimatedImage::getHeight() const
    {
        return height;
    }

    Color AnimatedImage::operator()(int x, int y) const
    {
        if (scheduler.imageFrames.empty())
        {
            throw std::runtime_error("AnimatedImage has no frames");
        }

        const auto &currentImage = images[scheduler.imageFrames[scheduler.currentIndex].imageIndex];
        return currentImage(x, y);
    }

    Color &AnimatedImage::operator()(int x, int y)
    {
        if (scheduler.imageFrames.empty())
        {
            throw std::runtime_error("AnimatedImage has no frames");
        }

        auto &currentImage = images[scheduler.imageFrames[scheduler.currentIndex].imageIndex];
        return currentImage(x, y);
    }

    void AnimatedImage::addImageFrame(const Image &image, float durationMs)
    {
        if (image.getWidth() != width || image.getHeight() != height)
        {
            throw std::invalid_argument("Bad frame dimensions");
        }

        images.push_back(image);
        scheduler.imageFrames.push_back({ static_cast<int>(images.size() - 1), durationMs });
    }

    void AnimatedImage::updateScheduler(float dt)
    {
        if (scheduler.imageFrames.empty())
        {
            return;
        }

        auto &imageFrame = scheduler.imageFrames[scheduler.currentIndex];
        scheduler.currentDurationMs += (dt * 1000.f);

        while (scheduler.currentDurationMs >= imageFrame.durationMs)
        {
            scheduler.currentDurationMs -= imageFrame.durationMs;
            scheduler.currentIndex = (scheduler.currentIndex + 1) % scheduler.imageFrames.size();
        }
    }
} // namespace Utils
