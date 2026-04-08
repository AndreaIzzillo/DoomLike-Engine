#pragma once

#include "game/objects/object.hpp"

namespace Game
{
    /**
     * @brief A class representing a segment in the game world.
     */
    class Segment : public IObject
    {
    public:
        Segment() = default;
        Segment(const Math::Point2 &start, const Math::Point2 &end);

        void update(float dt) override;
        void fixedUpdate(float dt) override;

        HitRecord hit(const Math::Ray &ray, float tMin,
                      float tMax) const override;

    private:
        Math::Point2 start;
        Math::Point2 end;
    };
} // namespace Game