#include "game/player.hpp"

namespace Game
{
    Player::Player(const Math::Point2 &position, const Math::Point2 &lookAt,
                   double fov, double focalDistance, unsigned resolution)
        : camera(position, lookAt, fov, focalDistance, resolution)
    {}

    const Math::Point2 &Player::getPosition() const
    {
        return camera.getPosition();
    }

    const Game::Camera &Player::getCamera() const
    {
        return camera;
    }

    void Player::update(double dt)
    {}

    void Player::move(const Math::Vector2 &delta)
    {
        camera.move(delta);
    }

    void Player::rotate(double angle)
    {
        camera.rotate(angle);
    }
} // namespace Game