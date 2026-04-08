#pragma once

#include "game/scene.hpp"

namespace Engine
{
    class InputManager
    {
    public:
        InputManager(Game::Scene *scene);

        void update();

    private:
        void playerRotation();
        void playerMovement();

    private:
        Game::Scene *scene;
    };
} // namespace Engine