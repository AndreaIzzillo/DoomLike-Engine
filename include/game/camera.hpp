#pragma once

#include "math/math.hpp"

namespace Game
{
    class Camera
    {
    public:
        Camera(const Math::Point2 &position, const Math::Point2 &lookAt,
               float fov, float focalDistance, unsigned resolution);
        ~Camera() = default;

        const Math::Point2 &getPosition() const;
        const Math::Vector2 &getForward() const;
        const Math::Vector2 &getRight() const;
        float getFov() const;
        float getFocalDistance() const;
        unsigned getResolution() const;

        void move(const Math::Vector2 &delta);
        void rotate(float angle);

        Math::Ray getRay(unsigned x) const;

    private:
        Math::Point2 position;

        Math::Vector2 forward;
        Math::Vector2 right;

        float fov;
        float focalDistance;

        unsigned resolution;

        Math::Point2 viewportPosition;
        float viewportWidth;
    };
} // namespace Game