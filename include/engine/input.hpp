#pragma once

#include "math/vector2.hpp"

namespace Engine
{

    struct InputState
    {
        Math::Vector2 inputDirection = { 0.0f, 0.0f };
        float rotationDirection = 0.0f;
    };

    /**
     * @brief Handles user input and translates it into game actions, such as
     * moving the player or rotating the camera.
     *
     * This class is only responsible for changing the state of the scene
     * components, such as the player, and does not directly interact with the
     * renderer or the game objects.
     *
     * Engine classes execution flow:
     * - Runner updates its InputManager (updates the scene components fields)
     * - Runner updates its Scene (calculates physics, logic, etc.)
     * - Renderer generates the current frame using RayCaster (based on the
     * updated scene)
     */
    class InputManager
    {
    public:
        InputManager();

        InputState update();

    private:
        float playerRotation();
        Math::Vector2 playerMovement();

    private:
    };
} // namespace Engine