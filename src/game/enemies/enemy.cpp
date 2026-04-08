#include "game/enemies/enemy.hpp"

#define ENEMY_RADIUS 0.5f

namespace Game
{
    Enemy::Enemy(const Math::Point2 &position)
        : position(position)
    {}

    void Enemy::update(float dt)
    {}

    void Enemy::fixedUpdate(float dt)
    {}

    HitRecord Enemy::hit(const Math::Ray &ray, float tMin, float tMax) const
    {
        return HitRecord{};
    }

    void Enemy::extrude(const Math::Ray &ray, const HitRecord &record,
                        const Player &player, Utils::Image &image,
                        unsigned x) const
    {
        throw std::runtime_error("Enemy extrusion not implemented yet");
    }
} // namespace Game
