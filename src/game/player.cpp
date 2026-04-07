#include "game/player.hpp"

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
    {}

    void Player::move(const Math::Vector2 &delta)
    {
        camera.move(delta);
    }

    void Player::rotate(float angle)
    {
        camera.rotate(angle);
    }
} // namespace Game