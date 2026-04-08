#pragma once

#include <memory>

#include "game/materials/material.hpp"
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
        const IMaterial *material = nullptr;
    };

    /**
     * @brief A class representing an object in the game world.
     *
     * Is an interface that defines the necessary methods for an object to be
     * updated and to be hit by a ray.
     */
    class IObject
    {
    public:
        virtual ~IObject() = default;

        virtual void update(float dt) = 0;
        virtual void fixedUpdate(float dt) = 0;

        virtual HitRecord hit(const Math::Ray &ray, float tMin,
                              float tMax) const = 0;

        virtual void setMaterial(std::shared_ptr<IMaterial> material)
        {
            this->material = material;
        }

    protected:
    public:
        std::shared_ptr<IMaterial> material;
    };
} // namespace Game
