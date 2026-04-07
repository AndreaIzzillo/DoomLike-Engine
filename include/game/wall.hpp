#pragma once

#include "game/segment.hpp"

namespace Game
{
    class Wall : public IObject
    {
    public:
        Wall() = default;
        Wall(const Segment &segment, float height);
        Wall(const Math::Point2 &start, const Math::Point2 &end, float height);

        float getHeight() const;

        void update(float dt) override;
        void fixedUpdate(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;

    private:
        Segment segment;
        float height;
    };
} // namespace Game