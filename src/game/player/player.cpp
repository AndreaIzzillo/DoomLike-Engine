#include "game/player/player.hpp"

#include <cfloat>
#include <cmath>

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

    const Math::Vector2 &Player::getVelocity() const
    {
        return velocity;
    }

    void Player::update(float dt)
    {}

    void Player::fixedUpdate(Math::Vector2 resolvedVelocity,
                             const std::vector<Math::Vector2> &hitNormals,
                             float dt)
    {
        /* Player fixed update is responsible for updating the player's position
         * and rotation based on the resolved velocity */
        velocity = resolvedVelocity;

        const auto &right = camera.getRight();
        const auto &forward = camera.getForward();

        for (const auto &normal : hitNormals)
        {
            /* Convert stored velocity to world space */
            Math::Vector2 worldVel = right * velocity.x + forward * velocity.y;
            float penetration = worldVel * normal;
            if (penetration < 0.f)
            {
                worldVel = worldVel - normal * penetration;
                /* Back to camera space */
                velocity = Math::Vector2(worldVel * right, worldVel * forward);
            }
        }

        camera.move(velocity * dt);
        camera.rotate(angularVelocity * rotationSpeed * dt);
    }

    void Player::BobCamera(float speed, float dt)
    {
        if (speed > FLT_EPSILON)
        {
            cameraShakingTime += speed * cameraShakingFrequency * dt;
            const float raw = std::sin(cameraShakingTime);
            const float step =
                -std::pow(std::abs(raw), 0.6f) * std::copysign(1.0f, raw);
            camera.setOffsetHeight(step * speed * cameraShakingAmplitude);
        }
        else
        {
            cameraShakingTime = 0.0f;
            camera.setOffsetHeight(camera.getOffsetHeight()
                                   * std::pow(0.99f, dt));
        }
    }

    Math::Vector2 Player::computeVelocity(Engine::InputState inputState,
                                          float dt)
    {
        Math::Vector2 intent = inputState.inputDirection;
        Math::Vector2 velocity_tmp = velocity;
        auto accelerationNorm = intent.norm();

        if (accelerationNorm > FLT_EPSILON)
        {
            velocity_tmp += intent * accelerationRate * dt;
        }
        else
        {
            velocity_tmp -= velocity_tmp * friction * dt;
        }

        auto velocityNorm = velocity_tmp.norm();

        if (velocityNorm >= maxSpeed)
            velocity_tmp = velocity_tmp.normalized() * maxSpeed;
        if (velocityNorm < 0.01f)
            velocity_tmp = Math::Vector2(0.0f, 0.0f);

        return velocity_tmp;
    }

    void Player::setAngularVelocity(float angularVelocity)
    {
        this->angularVelocity = angularVelocity;
    }

    void Player::nudge(const Math::Vector2 &delta)
    {
        camera.moveWorld(delta); // world space direct, pas de transform caméra
    }

} // namespace Game
