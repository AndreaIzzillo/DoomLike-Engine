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
     * @brief A struct representing the hit record of a ray-object intersection.
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

    struct CollisionHit
    {
        Math::Vector2 normal = { 0.f, 0.f };
        float penetration = 0.f;
        bool isColliding = false;
    };

    /**
     * @brief A class representing an object in the game world.
     *
     * Is an interface that defines the necessary methods for an object to be
     * updated and to be hit by a ray.
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
