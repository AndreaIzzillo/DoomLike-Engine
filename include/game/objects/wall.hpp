#pragma once

#include "game/objects/segment.hpp"

namespace Game
{
    /**
     * @brief A class representing a wall in the game world.
     */
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

        void extrude(const Math::Ray &ray, const HitRecord &record,
                     const Player &player, Utils::Image &image,
                     unsigned x) const override;

    private:
        Segment segment;
        float height;
    };
} // namespace Game