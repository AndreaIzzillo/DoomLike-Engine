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
               double fov, double focal_distance, unsigned resolution)
            : position(position)
            , forward((look_at - position).normalized())
            , fov(fov)
            , focal_distance(focal_distance)
            , resolution(resolution)
        {
            right = Math::Vector2(forward.y, -forward.x).normalized();
            viewport_width = 2.0 * focal_distance * tan(fov / 2.0);
            viewport_position = position + forward * focal_distance
                - right * (viewport_width / 2.0);
        };
        ~Camera() = default;

        Math::Ray get_ray(unsigned x) const
        {
            float pixel_size = viewport_width / resolution;
            Math::Point2 pixel_position =
                viewport_position + right * (pixel_size / 2.0 + x * pixel_size);
            return Math::Ray(position, pixel_position - position);
        }
    };
} // namespace Game