#pragma once

#include "game/camera.hpp"

namespace Game
{
    class Player
    {
    private:
        Game::Camera camera;

    public:
        Player(const Math::Point2 &position, const Math::Point2 &lookAt);
        ~Player() = default;

        const Math::Point2 &getPosition() const;
        const Game::Camera &getCamera() const;

        void update(float dt);
        void fixedUpdate(float dt);

        void move(const Math::Vector2 &delta);
        void rotate(float angle);
    };
} // namespace Game