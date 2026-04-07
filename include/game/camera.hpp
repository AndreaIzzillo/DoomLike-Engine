#pragma once

#include "math/math.hpp"

namespace Game
{
    class Camera
    {
    private:
        Math::Point2 position;

        Math::Vector2 forward;
        Math::Vector2 right;

        float fov;
        float focal_distance;

        unsigned resolution;

        Math::Point2 viewport_position;
        double viewport_width;

    public:
        Camera(const Math::Point2 &position, const Math::Point2 &look_at,
               double fov, double focal_distance, unsigned resolution);
        ~Camera() = default;

        const Math::Point2 &get_position() const;
        const Math::Vector2 &get_forward() const;
        const Math::Vector2 &get_right() const;
        float get_fov() const;
        float get_focal_distance() const;
        unsigned get_resolution() const;

        void move(const Math::Vector2 &delta);
        void rotate(double angle);

        Math::Ray get_ray(unsigned x) const;
    };
} // namespace Game