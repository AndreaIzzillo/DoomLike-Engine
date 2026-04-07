#include "game/player.hpp"

namespace Game
{
    Player::Player(const Math::Point2 &position, const Math::Point2 &look_at,
                   double fov, double focal_distance, unsigned resolution)
        : camera(position, look_at, fov, focal_distance, resolution)
    {}

    const Math::Point2 &Player::get_position() const
    {
        return camera.get_position();
    }

    const Game::Camera &Player::get_camera() const
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