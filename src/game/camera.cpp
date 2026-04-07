#include "game/camera.hpp"

#include "math/math.hpp"

namespace Game
{
    Camera::Camera(const Math::Point2 &position, const Math::Point2 &look_at,
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

    const Math::Point2 &Camera::get_position() const
    {
        return position;
    }

    const Math::Vector2 &Camera::get_forward() const
    {
        return forward;
    }

    const Math::Vector2 &Camera::get_right() const
    {
        return right;
    }

    float Camera::get_fov() const
    {
        return fov;
    }

    float Camera::get_focal_distance() const
    {
        return focal_distance;
    }

    unsigned Camera::get_resolution() const
    {
        return resolution;
    }

    void Camera::move(const Math::Vector2 &delta)
    {
        position += delta;
        viewport_position += delta;
    };

    void Camera::rotate(double angle)
    {
        auto rotation_matrix = Math::Matrix2x2(angle);

        forward = rotation_matrix * forward;
        right = rotation_matrix * right;

        viewport_position = position + forward * focal_distance
            - right * (viewport_width / 2.0);
    }

    Math::Ray Camera::get_ray(unsigned x) const
    {
        float pixel_size = viewport_width / resolution;
        Math::Point2 pixel_position =
            viewport_position + right * (pixel_size / 2.0 + x * pixel_size);
        return Math::Ray(position, pixel_position - position);
    }
} // namespace Game