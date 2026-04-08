#include "game/player/player.hpp"

#include "game/settings.hpp"

namespace Game
{
    Player::Player(const Math::Point2 &position, const Math::Point2 &lookAt)
        : camera(position, lookAt, Settings::get().cameraFov,
                 Settings::get().cameraFocalDistance,
                 Settings::get().cameraResolution)
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
        camera.move(velocity * movementSpeed * dt);
        camera.rotate(angularVelocity * rotationSpeed * dt);
    }

    void Player::setVelocity(
        const Math::Vector2 &velocity = Math::Vector2(0.0f, 0.0f))
    {
        this->velocity = velocity;
    }

    void Player::setAngularVelocity(float angularVelocity = 0.0f)
    {
        this->angularVelocity = angularVelocity;
    }
} // namespace Game