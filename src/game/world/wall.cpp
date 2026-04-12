#include "game/world/wall.hpp"

#include <algorithm>
#include <cfloat>
#include <cmath>

#include "game/world/sector.hpp"

namespace Game
{
    Wall::Wall(const Math::Point2 &start, const Math::Point2 &end,
               Sector *frontSector, Sector *backSector,
               std::shared_ptr<IMaterial> material)
    {
        if (frontSector == nullptr)
            throw std::invalid_argument("Front sector cannot be null");
        this->frontSector = frontSector;
        this->backSector = backSector;

        this->material = material;

        this->start = start;
        this->end = end;
    }

    Wall::Wall(const Math::Point2 &start, const Math::Point2 &end,
               float textureScaleX, float textureOffsetX, float textureScaleY,
               float textureOffsetY, Sector *frontSector, Sector *backSector,
               std::shared_ptr<IMaterial> material)
        : Wall(start, end, frontSector, backSector, material)
    {
        this->textureScaleX = textureScaleX;
        this->textureOffsetX = textureOffsetX;
        this->textureScaleY = textureScaleY;
        this->textureOffsetY = textureOffsetY;
    }

    const Math::Point2 &Wall::getStart() const
    {
        return start;
    }

    const Math::Point2 &Wall::getEnd() const
    {
        return end;
    }

    void Wall::setMaterial(std::shared_ptr<IMaterial> material)
    {
        this->material = material;
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
        rec.textureOffsetX = textureOffsetX;
        rec.textureScaleX = textureScaleX;
        rec.textureOffsetY = textureOffsetY;
        rec.textureScaleY = textureScaleY;
        /* Object information */
        rec.wall = this;
        rec.material = material.get();
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
