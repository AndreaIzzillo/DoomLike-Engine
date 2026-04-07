#pragma once

#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{
    class IObject;

    struct HitRecord
    {
        bool isHit;
        float t;
        Math::Point2 point;
        Math::Vector2 normal;
        const IObject *object;
    };

    class IObject
    {
    public:
        virtual ~IObject() = default;

        virtual void update(float dt) = 0;
        virtual void fixedUpdate(float dt) = 0;

        virtual HitRecord hit(const Math::Ray &ray, float tMin,
                              float tMax) const = 0;
    };
} // namespace Game
