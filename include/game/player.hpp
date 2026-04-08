#pragma once

#include "game/camera.hpp"

namespace Game
{
    class Player
    {
    public:
        Player(const Math::Point2 &position, const Math::Point2 &lookAt);
        ~Player() = default;

        const Math::Point2 &getPosition() const;
        const Game::Camera &getCamera() const;

        void update(float dt);
        void fixedUpdate(float dt);

        /*
         * Player physics
         */
        void setVelocity(const Math::Vector2 &velocity);
        void setAngularVelocity(float angularVelocity);

    private:
        /*
         * Player camera
         */
        Game::Camera camera;

        /*
         * Player physics
         */
        // Position is stored in the camera
        Math::Vector2 velocity = Math::Vector2(0.0f, 0.0f);
        float movementSpeed = 2.0f;

        float angularVelocity = 0.0f;
        float rotationSpeed = 2.0f;
    };
} // namespace Game