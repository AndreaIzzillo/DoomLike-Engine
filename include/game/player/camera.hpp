#pragma once

#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{
    /**
     * @brief A class representing the camera in the game world.
     *
     * The camera is responsible of the player's view of the world, and is used
     * to generate rays for rendering. The camera contains the player's
     * position, orientation, field of view, focal distance and resolution.
     */
    class Camera
    {
    public:
        Camera(const Math::Point2 &position, const Math::Point2 &lookAt,
               float fov, float focalDistance, unsigned resolution);
        ~Camera() = default;

        /* Getters */
        const Math::Point2 &getPosition() const;
        float getCameraHeight() const;
        const Math::Vector2 &getForward() const;
        const Math::Vector2 &getRight() const;
        float getFov() const;
        float getFocalDistance() const;
        unsigned getResolution() const;
        float getViewportWidth() const;
        float getOffsetHeight() const;

        /* Setters */
        void setOffsetHeight(float offsetHeight);

        /* Camera physics */
        void move(const Math::Vector2 &delta);
        void rotate(float angle);

        /* Ray generation (for RayCaster.hpp) */
        Math::Ray getRay(unsigned x) const;

    private:
        /* Camera / Player position */
        Math::Point2 position;
        float cameraHeight;
        float offsetHeight;

        /* Camera orientation / plane */
        Math::Vector2 forward;
        Math::Vector2 right;

        /* Camera properties */
        float fov;
        float focalDistance;
        unsigned resolution;

        /* Viewport properties */
        Math::Point2 viewportPosition;
        float viewportWidth;
    };
} // namespace Game