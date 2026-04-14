#include "game/world/wall.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include "game/world/sector.hpp"

namespace Game
{
    Wall::Wall(const Math::Point2 &start, const Math::Point2 &end,
               Sector *frontSector, Sector *backSector,
               std::shared_ptr<IMaterial> material,
               std::shared_ptr<IMaterial> upperMaterial,
               std::shared_ptr<IMaterial> lowerMaterial)
    {
        if (frontSector == nullptr)
            throw std::invalid_argument("Front sector cannot be null");
        this->frontSector = frontSector;
        this->backSector = backSector;

        this->material = material;
        this->upperMaterial = upperMaterial;
        this->lowerMaterial = lowerMaterial;

        this->start = start;
        this->end = end;
    }

    std::unique_ptr<Wall> Wall::createPlain(const Math::Point2 &start,
                                            const Math::Point2 &end,
                                            Sector *frontSector,
                                            std::shared_ptr<IMaterial> material)
    {
        return std::make_unique<Wall>(start, end, frontSector, nullptr,
                                      material, nullptr, nullptr);
    }

    std::unique_ptr<Wall>
    Wall::createPortal(const Math::Point2 &start, const Math::Point2 &end,
                       Sector *frontSector, Sector *backSector,
                       std::shared_ptr<IMaterial> upperMaterial,
                       std::shared_ptr<IMaterial> lowerMaterial)
    {
        return std::make_unique<Wall>(start, end, frontSector, backSector,
                                      nullptr, upperMaterial, lowerMaterial);
    }

    const Math::Point2 &Wall::getStart() const
    {
        return start;
    }

    const Math::Point2 &Wall::getEnd() const
    {
        return end;
    }

    bool Wall::isPortal() const
    {
        return backSector != nullptr;
    }

    void Wall::setTextureTransform(float scaleX, float offsetX, float scaleY,
                                   float offsetY)
    {
        textureTransform = { scaleX, offsetX, scaleY, offsetY };
    }

    void Wall::setUpperTextureTransform(float scaleX, float offsetX,
                                        float scaleY, float offsetY)
    {
        upperTextureTransform = { scaleX, offsetX, scaleY, offsetY };
    }

    void Wall::setLowerTextureTransform(float scaleX, float offsetX,
                                        float scaleY, float offsetY)
    {
        lowerTextureTransform = { scaleX, offsetX, scaleY, offsetY };
    }

    float Wall::stepHeight() const
    {
        if (!isPortal())
            return 0.f;
        return std::abs(frontSector->getFloorHeight()
                        - backSector->getFloorHeight());
    }

    HitRecord Wall::hit(const Math::Ray &ray, float tMin, float tMax) const
    {
        Math::Vector2 r = ray.direction;
        Math::Vector2 s = end - start;
        Math::Vector2 diff = start - ray.origin;
        float denom = r ^ s;

        if (std::fabs(denom) < FLT_EPSILON)
            return HitRecord{};

        float t = (diff ^ s) / denom;
        float u = (diff ^ r) / denom;

        if (t < tMin || t > tMax || u < 0.f || u > 1.f)
            return HitRecord{};

        /* HitRecord settings */
        HitRecord rec;
        rec.isHit = true;
        /* Hit point information */
        rec.t = t;
        rec.point = ray.at(t);
        Math::Vector2 segDir = s.normalized();
        rec.normal = Math::Vector2(-segDir.y, segDir.x);
        /* Texture mapping information */
        rec.u = u;
        rec.textureTransform = textureTransform;
        rec.upperTextureTransform = upperTextureTransform;
        rec.lowerTextureTransform = lowerTextureTransform;
        /* Object information */
        rec.wall = this;
        rec.material = material.get();
        rec.upperMaterial = upperMaterial.get();
        rec.lowerMaterial = lowerMaterial.get();
        /* Sector information */
        rec.frontSector = frontSector;
        rec.backSector = backSector;

        return rec;
    }

    Math::Point2 Wall::closestPoint(const Math::Point2 &p) const
    {
        Math::Vector2 seg = end - start;
        Math::Vector2 toP = p - start;
        float t = (toP * seg) / (seg * seg);
        t = std::clamp(t, 0.f, 1.f);
        return start + seg * t;
    }
} // namespace Game
