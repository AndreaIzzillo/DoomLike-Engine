#pragma once

#include "game/interfaces/iwall.hpp"

namespace Game
{
    /**
     * @brief Concrete line-segment wall with material and ray intersection.
     *
     * It provides hit information (distance, normal, material context) used by
     * rendering and collision checks.
     */
    class Wall : public IWall
    {
    public:
        Wall() = default;
        Wall(const Math::Point2 &start, const Math::Point2 &end);

        void update(float dt) override;
        void fixedUpdate(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;
    };
} // namespace Game
