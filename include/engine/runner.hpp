#pragma once

#include "engine/collision_manager.hpp"
#include "engine/input_manager.hpp"
#include "engine/renderer.hpp"
#include "game/scene/scene.hpp"

namespace Engine
{
    class Runner
    {
    public:
        Runner(std::unique_ptr<Game::Scene> scene);
        ~Runner() = default;

        void addWall(std::unique_ptr<Game::Wall> wall);
        void addSprite(std::unique_ptr<::Game::Sprite> sprite);

        void run();

    private:
        void handleEvents();
        void update(sf::Time dt);
        void fixedUpdate(sf::Time dt);

    private:
        /* Timing management for fixed updates */
        sf::Clock clock;
        sf::Time accumulatedTime;

        /* Internal engine systems */
        Renderer renderer;
        InputManager inputManager;
        CollisionManager collisionManager;

        /* Game scene (unique) */
        std::unique_ptr<Game::Scene> scene;
    };
} // namespace Engine
