#pragma once

#include "engine/collision_manager.hpp"
#include "engine/input_manager.hpp"
#include "engine/renderer.hpp"
#include "game/scene/scene.hpp"

namespace Engine
{
    /**
     * @brief Drives the main loop and orchestrates engine subsystems.
     *
     * Per frame it handles window events, fetches input, updates the scene,
     * runs fixed-step simulation with collision resolution, then renders the
     * resulting state.
     */
    class Runner
    {
    public:
        Runner(std::unique_ptr<Game::Scene> scene);
        ~Runner() = default;

        void addWall(std::unique_ptr<Game::Wall> wall);

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
