#pragma once

#include "game/objects/object.hpp"

namespace Game
{
    /**
     * @brief TODO
     */
    class Enemy : public IObject
    {
    public:
        Enemy() = default;
        Enemy(const Math::Point2 &position);

        void update(float dt) override;
        void fixedUpdate(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;

    private:
        Math::Point2 position;
    };
} // namespace Game