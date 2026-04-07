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
        hitRecord hit(const Math::Ray &ray, float t_min,
                      float t_max) const override;

    private:
        Math::Point2 start_;
        Math::Point2 end_;
    };
} // namespace Game