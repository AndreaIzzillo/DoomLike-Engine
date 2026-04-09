#include "game/player/player.hpp"

#include <cfloat>
#include <cstdlib>

#include "game/settings.hpp"

namespace Game
{
    Player::Player(const Math::Point2 &position, const Math::Point2 &lookAt)
        : camera(position, lookAt, Settings::get().cameraFov,
                 Settings::get().cameraFocalDistance,
                 Settings::get().windowWidth)
        , velocity(Math::Vector2(0.0f, 0.0f))
        , acceleration(Math::Vector2(0.0f, 0.0f))
        , angularVelocity(0.0f)
    {}

    const Math::Point2 &Player::getPosition() const
    {
        return camera.getPosition();
    }

    const Game::Camera &Player::getCamera() const
    {
        return camera;
    }

    void Player::update(float dt)
    {}

    void Player::fixedUpdate(float dt)
    {
        auto accelerationNorm = acceleration.norm();
        if (accelerationNorm > FLT_EPSILON)
            velocity += acceleration * accelerationRate * dt;
        else
            velocity -= velocity * friction * dt;

        auto velocityNorm = velocity.norm();
        if (velocityNorm >= maxSpeed)
            velocity = velocity.normalized() * maxSpeed;
        if (velocityNorm < 0.1f)
            velocity = Math::Vector2(0.0f, 0.0f);

        camera.move(velocity * dt);
        camera.rotate(angularVelocity * rotationSpeed * dt);

        const float speed = velocity.norm();

        if (speed > FLT_EPSILON)
        {
            cameraShakingTime += speed * cameraShakingFrequency * dt;
            const float raw = std::sin(cameraShakingTime);
            const float step =
                -std::pow(std::abs(raw), 0.6f) * std::copysign(1.0f, raw);
            const float shaking = step * speed * cameraShakingAmplitude;
            camera.setOffsetHeight(shaking);
        }
        else
        {
            cameraShakingTime = 0.0f;
            camera.setOffsetHeight(camera.getOffsetHeight() * std::pow(0.99f, dt));
        }
    }

    void Player::setAcceleration(const Math::Vector2 &acceleration)
    {
        this->acceleration = acceleration;
    }

    void Player::setAngularVelocity(float angularVelocity)
    {
        this->angularVelocity = angularVelocity;
    }

    void Player::setVelocity(const Math::Vector2 &velocity)
    {
        this->velocity = velocity;
    }
} // namespace Game