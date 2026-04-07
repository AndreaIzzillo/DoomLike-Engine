#include "game/enemy.hpp"

#define ENEMY_RADIUS 0.5f

namespace Game
{
    Enemy::Enemy(const Math::Point2 &position)
        : position(position)
    {}

    void Enemy::update(float dt)
    {}

    HitRecord Enemy::hit(const Math::Ray &ray, float t_min, float t_max) const
    {
        return HitRecord{};
    }
} // namespace Game