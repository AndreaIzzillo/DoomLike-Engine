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

    private:
        /* Player camera */
        Game::Camera camera;

        /* Player physics */
        Math::Vector2 velocity;
        float angularVelocity;

        /* Player properties */
        const float movementSpeed = 2.0f;
        const float rotationSpeed = 2.0f;
    };
} // namespace Game