#pragma once

#include "engine/input_manager.hpp"
#include "game/player/camera.hpp"
#include "math/vector2.hpp"

namespace Game
{
    /**
     * @brief Encapsulates controllable actor movement and camera state.
     *
     * It derives velocity intent from input, then applies collision-resolved
     * velocity and angular speed during fixed updates, including camera
     * bobbing.
     */
    class Player
    {
    public:
        Player(const Math::Point2 &position, const Math::Point2 &lookAt);
        ~Player() = default;

        /* Getters */
        const Math::Point2 &getPosition() const;
        const Math::Vector2 &getVelocity() const;
        const Game::Camera &getCamera() const;

        void update(float dt);
        void fixedUpdate(Math::Vector2 resolvedIntent, float dt);

        /* Player physics */
        void setAngularVelocity(float angularVelocity);

        Math::Vector2 computeVelocity(Engine::InputState inputState, float dt);

    private:
        /* Player camera */
        Game::Camera camera;

        /* Player physics */
        Math::Vector2 velocity;
        Math::Vector2 acceleration;
        float angularVelocity;

        /* Camera Shaking properties */
        float cameraShakingTime = 0.0f;
        static constexpr float cameraShakingFrequency = 2.25f;
        static constexpr float cameraShakingAmplitude = 0.01f;

        /* Spatial properties */
        static constexpr float maxSpeed = 5.f;
        static constexpr float accelerationRate = 30.f;
        static constexpr float friction = 7.f;

        /* Angular properties */
        static constexpr float rotationSpeed = 2.f;
    };
} // namespace Game
