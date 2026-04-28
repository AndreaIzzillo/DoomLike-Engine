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

    struct TextureTransform
    {
        float scaleX = 1.f;
        float offsetX = 0.f;
        float scaleY = 1.f;
        float offsetY = 0.f;
    };

    struct HitRecord
    {
        /* Intersection properties */
        bool isHit = false;
        float t = 0.f;
        Math::Point2 point = { 0.f, 0.f };
        Math::Vector2 normal = { 0.f, 0.f };

        /* Other wall properties for texture mapping */
        float u = 0.f;
        TextureTransform textureTransform;
        TextureTransform upperTextureTransform;
        TextureTransform lowerTextureTransform;

        /* Wall and material pointers */
        const Wall *wall = nullptr;
        const IMaterial *material = nullptr;
        const IMaterial *upperMaterial = nullptr;
        const IMaterial *lowerMaterial = nullptr;

        /* Sector */
        const Sector *frontSector = nullptr;
        const Sector *backSector = nullptr;
    };

    class Wall
    {
    public:
        Wall() = default;
        Wall(const Math::Point2 &start, const Math::Point2 &end, Sector *frontSector,
             Sector *backSector = nullptr, std::shared_ptr<IMaterial> material = nullptr,
             std::shared_ptr<IMaterial> upperMaterial = nullptr,
             std::shared_ptr<IMaterial> lowerMaterial = nullptr);
        ~Wall() = default;

        /* Builders to make our lives easier */
        static std::unique_ptr<Wall> createPlain(const Math::Point2 &start, const Math::Point2 &end,
                                                 Sector *frontSector,
                                                 std::shared_ptr<IMaterial> material);
        static std::unique_ptr<Wall> createPortal(const Math::Point2 &start,
                                                  const Math::Point2 &end, Sector *frontSector,
                                                  Sector *backSector,
                                                  std::shared_ptr<IMaterial> upperMaterial,
                                                  std::shared_ptr<IMaterial> lowerMaterial);

        /* Getters */
        const Math::Point2 &getStart() const;
        const Math::Point2 &getEnd() const;

        void setTextureTransform(float scaleX, float offsetX, float scaleY, float offsetY);
        void setUpperTextureTransform(float scaleX, float offsetX, float scaleY, float offsetY);
        void setLowerTextureTransform(float scaleX, float offsetX, float scaleY, float offsetY);

        /* Ray-wall intersection */
        HitRecord hit(const Math::Ray &ray, float tMin, float tMax) const;

        /* Closest point on the wall segment to a given point (for push-out) */
        Math::Point2 closestPoint(const Math::Point2 &p) const;

        /* Portal related methods */
        bool isPortal() const;
        bool canTraverse(float playerHeight, float jumpHeight, const Sector *from) const;

        /* return the height of the step if its a Portal */
        float stepHeight() const;

    protected:
        Sector *frontSector;
        Sector *backSector = nullptr;

        /* Plain Wall Material */
        std::shared_ptr<IMaterial> material;

        /* Portal Material */
        std::shared_ptr<IMaterial> upperMaterial;
        std::shared_ptr<IMaterial> lowerMaterial;

        Math::Point2 start;
        Math::Point2 end;

        TextureTransform textureTransform;
        TextureTransform upperTextureTransform;
        TextureTransform lowerTextureTransform;
    };
} // namespace Game
