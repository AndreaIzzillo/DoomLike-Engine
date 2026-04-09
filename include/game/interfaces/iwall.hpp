#pragma once

#include <memory>

#include "game/interfaces/imaterial.hpp"
#include "game/settings.hpp"
#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{

    class IWall;

    /**
     * @brief Ray-wall intersection payload shared by rendering and collisions.
     *
     * Carries geometric hit data plus wall/material context used for texture
     * sampling and wall projection.
     */
    struct HitRecord
    {
        /* Intersection properties */
        bool isHit = false;
        float t = 0.f;
        Math::Point2 point = { 0.f, 0.f };
        Math::Vector2 normal = { 0.f, 0.f };

        /* Floor and ceiling settings for rendering */
        float floor = Settings::get().worldFloor;
        float ceiling = Settings::get().worldCeiling;

        /* Other wall properties for rendering */
        const Math::Point2 *wallStart = nullptr;
        const Math::Point2 *wallEnd = nullptr;
        float hitDistance = 0.f;

        /* Object and material pointers */
        const IWall *object = nullptr;
        const IMaterial *material = nullptr;
    };

    /**
     * @brief Generic overlap-collision result (normal and penetration depth).
     *
     * Can be used by penetration-based solvers when broad collision queries
     * return an overlapping state.
     */
    struct CollisionHit
    {
        Math::Vector2 normal = { 0.f, 0.f };
        float penetration = 0.f;
        bool isColliding = false;
    };

    /**
     * @brief Interface for world wall geometry participating in simulation.
     *
     * Implementations can be updated over time and must provide ray-hit data
     * for both rendering (RayCaster) and movement blocking (CollisionManager).
     */
    class IWall
    {
    public:
        virtual ~IWall() = default;

        virtual void update(float dt) = 0;
        virtual void fixedUpdate(float dt) = 0;

        virtual const Math::Point2 &getStart() const
        {
            return start;
        }

        virtual const Math::Point2 &getEnd() const
        {
            return end;
        }

        /* Material */
        virtual void setMaterial(std::shared_ptr<IMaterial> material)
        {
            this->material = material;
        }

        /* Ray-Object Intersection */
        virtual HitRecord hit(const Math::Ray &ray, float tMin,
                              float tMax) const = 0;

    protected:
        std::shared_ptr<IMaterial> material;
        Math::Point2 start;
        Math::Point2 end;
    };
} // namespace Game
