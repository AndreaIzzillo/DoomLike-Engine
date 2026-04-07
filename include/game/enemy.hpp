#pragma once

#include "game/object.hpp"

namespace Game
{
    class Enemy : public IObject
    {
    public:
        Enemy() = default;
        Enemy(const Math::Point2 &position);

        void update(float dt) override;
        HitRecord hit(const Math::Ray &ray, float t_min,
                      float t_max) const override;

    private:
        Math::Point2 position;
    };
} // namespace Game