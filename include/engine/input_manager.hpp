#pragma once

#include "math/vector2.hpp"

namespace Engine
{

    struct InputState
    {
        float rotationDirection = 0.0f;
        Math::Vector2 inputDirection = { 0.0f, 0.0f };
    };

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
