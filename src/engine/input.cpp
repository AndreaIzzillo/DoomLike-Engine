#include "engine/input.hpp"

#include <SFML/Window/Keyboard.hpp>

#include "game/player/player.hpp"
#include "math/vector2.hpp"

#define W sf::Keyboard::Scancode::W
#define A sf::Keyboard::Scancode::A
#define S sf::Keyboard::Scancode::S
#define D sf::Keyboard::Scancode::D

#define LEFT sf::Keyboard::Scancode::Left
#define RIGHT sf::Keyboard::Scancode::Right

namespace Engine
{
    InputManager::InputManager(Game::Scene *scene)
        : scene(scene)
    {}

    void InputManager::update()
    {
        playerMovement();
        playerRotation();
    }

    void InputManager::playerMovement()
    {
        Game::Player &player = scene->getPlayer();

        Math::Vector2 velocity(0.0f, 0.0f);
        if (sf::Keyboard::isKeyPressed(W))
        {
            velocity.y += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(S))
        {
            velocity.y -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(A))
        {
            velocity.x -= 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(D))
        {
            velocity.x += 1.0f;
        }

        player.setVelocity(velocity.normalized());
    }

    void InputManager::playerRotation()
    {
        Game::Player &player = scene->getPlayer();

        float angularVelocity = 0.0f;
        if (sf::Keyboard::isKeyPressed(LEFT))
        {
            angularVelocity += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(RIGHT))
        {
            angularVelocity -= 1.0f;
        }

        player.setAngularVelocity(angularVelocity);
    }
} // namespace Engine