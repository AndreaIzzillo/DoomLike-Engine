#include "engine/input_manager.hpp"

#include <SFML/Window/Keyboard.hpp>

#define W sf::Keyboard::Scancode::W
#define A sf::Keyboard::Scancode::A
#define S sf::Keyboard::Scancode::S
#define D sf::Keyboard::Scancode::D

#define LEFT sf::Keyboard::Scancode::Left
#define RIGHT sf::Keyboard::Scancode::Right

namespace Engine
{
    InputManager::InputManager()
    {}

    InputState InputManager::fetchInputState()
    {
        return { playerRotation(), playerMovement() };
    }

    Math::Vector2 InputManager::playerMovement()
    {
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

        return velocity.normalized();
    }

    float InputManager::playerRotation()
    {
        float rotationDirection = 0.0f;
        if (sf::Keyboard::isKeyPressed(LEFT))
        {
            rotationDirection += 1.0f;
        }
        if (sf::Keyboard::isKeyPressed(RIGHT))
        {
            rotationDirection -= 1.0f;
        }
        return rotationDirection;
    }
} // namespace Engine
