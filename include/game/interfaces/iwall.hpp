#pragma once

#include <memory>

#include "game/interfaces/imaterial.hpp"
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

        /* Other wall properties for texture mapping */
        float u = 0.f;
        float textureScaleX = 1.f;
        float textureOffsetX = 0.f;
        float textureScaleY = 1.f;
        float textureOffsetY = 0.f;

        /* Wall and material pointers */
        const IWall *wall = nullptr;
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

        /* Ray-wall intersection */
        virtual HitRecord hit(const Math::Ray &ray, float tMin,
                              float tMax) const = 0;

    protected:
        std::shared_ptr<IMaterial> material;

        Math::Point2 start;
        Math::Point2 end;

        float textureScaleX = 1.f;
        float textureOffsetX = 0.f;
        float textureScaleY = 1.f;
        float textureOffsetY = 0.f;
    };
} // namespace Game
