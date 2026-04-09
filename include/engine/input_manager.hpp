#pragma once

#include "math/vector2.hpp"

namespace Engine
{

    /**
     * @brief Snapshot of player input for one frame.
     *
     * Filled by InputManager and consumed by Scene to drive movement intent
     * (`inputDirection`) and camera rotation intent (`rotationDirection`).
     */
    struct InputState
    {
        float rotationDirection = 0.0f;
        Math::Vector2 inputDirection = { 0.0f, 0.0f };
    };

    /**
     * @brief Reads keyboard state and produces gameplay input commands.
     *
     * Runner pulls an InputState each frame, then Scene consumes it during
     * update/fixedUpdate to drive player movement and rotation intent.
     */
    class InputManager
    {
    public:
        InputManager();

        InputState fetchInputState();

    private:
        float playerRotation();
        Math::Vector2 playerMovement();
    };
} // namespace Engine
