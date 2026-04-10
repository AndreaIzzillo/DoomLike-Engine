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
    class PlainWall : public IWall
    {
    public:
        PlainWall() = default;
        PlainWall(const Math::Point2 &start, const Math::Point2 &end);
        PlainWall(const Math::Point2 &start, const Math::Point2 &end,
                  float textureScaleX, float textureOffsetX,
                  float textureScaleY, float textureOffsetY);

        void update(float dt) override;
        void fixedUpdate(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;
    };
} // namespace Game
