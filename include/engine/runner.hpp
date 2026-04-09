#pragma once

#include "engine/input.hpp"
#include "engine/renderer.hpp"
#include "game/objects/object.hpp"
#include "game/scene/scene.hpp"

namespace Engine
{
    /**
     * @brief A class for managing the main game loop and coordinating the
     different engine systems.
     *
     * The Runner class is responsible for initializing the game systems,
     * handling events, updating the game state, and rendering the scene.
     *
     * Engine classes execution flow:
     * - Runner updates its InputManager (updates the scene components fields)
     * - Runner updates its Scene (calculates physics, logic, etc.)
     * - Renderer generates the current frame using RayCaster (based on the
     updated scene)
     */
    class Runner
    {
    public:
        Runner(std::unique_ptr<::Game::Scene> scene);
        ~Runner() = default;

        void addObject(std::unique_ptr<::Game::IWall> object);

        void run();

    private:
        void handleEvents();
        void update(sf::Time dt);
        void fixedUpdate(sf::Time dt);

    private:
        /* Timing management for fixed updates */
        sf::Clock clock;
        sf::Time accumulatedTime;

        /* Internal engine systems with their own update loops (unique) */
        Renderer renderer;
        InputManager inputManager;

        /* Game scene (unique) */
        std::unique_ptr<::Game::Scene> scene;
    };
} // namespace Engine