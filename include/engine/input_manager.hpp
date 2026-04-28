#pragma once

#include "math/vector2.hpp"

namespace Engine
{
    struct InputState
    {
        float rotationDirection = 0.f;
        Math::Vector2 inputDirection = { 0.f, 0.f };
    };

    class InputManager
    {
    public:
        InputManager();

        InputState fetchInputState();

    private:
        /* Input fetching methods */
        float playerRotation();
        Math::Vector2 playerMovement();
    };
} // namespace Engine
