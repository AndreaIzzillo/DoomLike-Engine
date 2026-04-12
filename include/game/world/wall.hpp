#pragma once

#include <memory>

#include "game/interfaces/imaterial.hpp"
#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{
    class Wall;
    class Sector;

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
        const Wall *wall = nullptr;
        const IMaterial *material = nullptr;

        /* Sector */
        const Sector *frontSector = nullptr;
        const Sector *backSector = nullptr;
    };

    /**
     * @brief Concrete line-segment wall with material and ray intersection.
     *
     * It provides hit information (distance, normal, material context) used by
     * rendering and collision checks.
     */
    class Wall
    {
    public:
        Wall() = default;
        Wall(const Math::Point2 &start, const Math::Point2 &end,
             Sector *frontSector, Sector *backSector = nullptr,
             std::shared_ptr<IMaterial> material = nullptr);
        Wall(const Math::Point2 &start, const Math::Point2 &end,
             float textureScaleX, float textureOffsetX, float textureScaleY,
             float textureOffsetY, Sector *frontSector,
             Sector *backSector = nullptr,
             std::shared_ptr<IMaterial> material = nullptr);
        ~Wall() = default;

        /* Getters */
        const Math::Point2 &getStart() const;
        const Math::Point2 &getEnd() const;

        /* Setters */
        void setMaterial(std::shared_ptr<IMaterial> material);

        /* Ray-wall intersection */
        HitRecord hit(const Math::Ray &ray, float tMin, float tMax) const;

        /* Closest point on the wall segment to a given point (for push-out) */
        Math::Point2 closestPoint(const Math::Point2 &p) const;

    protected:
        Sector *frontSector;
        Sector *backSector = nullptr;

        std::shared_ptr<IMaterial> material;

        Math::Point2 start;
        Math::Point2 end;

        float textureScaleX = 1.f;
        float textureOffsetX = 0.f;
        float textureScaleY = 1.f;
        float textureOffsetY = 0.f;
    };
} // namespace Game
