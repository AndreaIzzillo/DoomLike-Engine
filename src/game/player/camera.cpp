#include "game/player/camera.hpp"

#include "math/matrix2x2.hpp"

namespace Game
{
    Camera::Camera(const Math::Point2 &position, const Math::Point2 &lookAt,
                   float fov, float focalDistance, unsigned resolution)
        : position(position)
        , cameraHeight(0.5f)
        , forward((lookAt - position).normalized())
        , fov(fov)
        , focalDistance(focalDistance)
        , resolution(resolution)
    {
        right = Math::Vector2(forward.y, -forward.x).normalized();
        viewportWidth = 2.0 * focalDistance * tan(fov / 2.0);
        viewportPosition =
            position + forward * focalDistance - right * (viewportWidth / 2.0);
    };

    const Math::Point2 &Camera::getPosition() const
    {
        return position;
    }

    float Camera::getCameraHeight() const
    {
        return cameraHeight;
    }

    const Math::Vector2 &Camera::getForward() const
    {
        return forward;
    }

    const Math::Vector2 &Camera::getRight() const
    {
        return right;
    }

    float Camera::getFov() const
    {
        return fov;
    }

    float Camera::getFocalDistance() const
    {
        return focalDistance;
    }

    unsigned Camera::getResolution() const
    {
        return resolution;
    }

    float Camera::getViewportWidth() const
    {
        return viewportWidth;
    }

    float Camera::getOffsetHeight() const
    {
        return offsetHeight;
    }

    void Camera::setOffsetHeight(float offsetHeight)
    {
        this->offsetHeight = offsetHeight;
    }

    void Camera::move(const Math::Vector2 &delta)
    {
        Math::Vector2 movement = right * delta.x + forward * delta.y;
        position += movement;
        viewportPosition += movement;
    };

    void Camera::rotate(float angle)
    {
        auto rotation_matrix = Math::Matrix2x2(angle);

        forward = rotation_matrix * forward;
        right = rotation_matrix * right;

        viewportPosition =
            position + forward * focalDistance - right * (viewportWidth / 2.0);
    }

    Math::Ray Camera::getRay(unsigned x) const
    {
        float pixel_size = viewportWidth / resolution;
        Math::Point2 pixel_position =
            viewportPosition + right * (pixel_size / 2.0 + x * pixel_size);
        return Math::Ray(position, pixel_position - position);
    }
} // namespace Game