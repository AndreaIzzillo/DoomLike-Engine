#pragma once

#include "game/world/sector.hpp"
#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{
    /**
     * @brief First-person camera bound to player spatial state.
     *
     * It maintains orientation vectors and projection parameters, and exposes
     * per-column rays consumed by the renderer raycasting pass.
     */
    class Camera
    {
    public:
        Camera(const Math::Point2 &position, const Math::Point2 &lookAt, float fov,
               float focalDistance, int resolution);
        ~Camera() = default;

        /* Getters */
        const Math::Point2 &getPosition() const;
        float getCameraHeight() const;
        const Math::Vector2 &getForward() const;
        const Math::Vector2 &getRight() const;
        float getFov() const;
        float getFocalDistance() const;
        int getResolution() const;
        float getViewportWidth() const;
        float getOffsetHeight() const;
        const Sector *getCurrentSector() const;

        /* Setters */
        void setOffsetHeight(float offsetHeight);
        void setCurrentSector(const Sector *sector, const float playerSize,
                              const float upperHitBox);

        /* Camera physics */
        void move(const Math::Vector2 &delta);

        void rotate(float angle);

        /* Ray generation (for RayCaster.hpp) */
        Math::Ray getRay(int x) const;

    private:
        /* Camera / Player position */
        Math::Point2 position;
        float cameraHeight;
        float offsetHeight;
        const Sector *currentSector;

        /* Camera orientation / plane */
        Math::Vector2 forward;
        Math::Vector2 right;

        /* Camera properties */
        float fov;
        float focalDistance;
        int resolution;

        /* Viewport properties */
        Math::Point2 viewportPosition;
        float viewportWidth;
    };
} // namespace Game
