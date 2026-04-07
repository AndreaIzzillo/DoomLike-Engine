#pragma once

#include "game/camera.hpp"
#include "math/math.hpp"

namespace Game
{
    class Player
    {
    private:
        Game::Camera camera;

    public:
        Player(const Math::Point2 &position, const Math::Point2 &look_at,
               double fov, double focal_distance, unsigned resolution);
        ~Player() = default;

        const Math::Point2 &get_position() const;
        const Game::Camera &get_camera() const;

        void update(double dt);

        void move(const Math::Vector2 &delta);
        void rotate(double angle);
    };
} // namespace Game