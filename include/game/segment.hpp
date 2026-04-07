#pragma once

#include "game/object.hpp"

namespace Game
{
    class Segment : public IObject
    {
    public:
        Segment() = default;
        Segment(const Math::Point2 &start, const Math::Point2 &end);

        void update(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;

    private:
        Math::Point2 start;
        Math::Point2 end;
    };
} // namespace Game