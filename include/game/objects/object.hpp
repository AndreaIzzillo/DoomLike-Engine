#pragma once

#include "math/point2.hpp"
#include "math/ray.hpp"
#include "math/vector2.hpp"

namespace Game
{
    class IObject;

    /**
     * @brief A struct representing the hit record of a ray-object intersection.
     */
    struct HitRecord
    {
        bool isHit = false;
        float t = 0.f;
        Math::Point2 point = { 0.f, 0.f };
        Math::Vector2 normal = { 0.f, 0.f };
        const IObject *object = nullptr;
    };

    /**
     * @brief A class representing an object in the game world.
     */
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
