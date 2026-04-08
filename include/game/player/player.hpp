#pragma once

#include "game/player/camera.hpp"

namespace Game
{
    /**
     * @brief A class representing the player in the game world.
     */
    class Player
    {
    public:
        Player(const Math::Point2 &position, const Math::Point2 &lookAt);
        ~Player() = default;

        /* Getters */
        const Math::Point2 &getPosition() const;
        const Game::Camera &getCamera() const;

        void update(float dt);
        void fixedUpdate(float dt);

        /* Player physics */
        void setVelocity(const Math::Vector2 &velocity);
        void setAngularVelocity(float angularVelocity);
        void setAcceleration(const Math::Vector2 &acceleration);

    private:
        /* Player camera */
        Game::Camera camera;

        /* Player physics */
        Math::Vector2 velocity;
        Math::Vector2 acceleration;
        float angularVelocity;

        /* Player properties */

        /* Spatial properties */
        static constexpr float maxSpeed = 1.0f;
        static constexpr float accelerationRate = 2.f;
        static constexpr float friction = 0.9f;

        /* angular properties */
        static constexpr float rotationSpeed = 1.0f;
    };
} // namespace Game