#pragma once

#include "engine/renderer.hpp"
#include "game/object.hpp"
#include "game/scene.hpp"

namespace Engine
{
    class Runner
    {
    public:
        Runner(std::unique_ptr<::Game::Scene> scene);
        ~Runner() = default;

        void addObject(std::unique_ptr<::Game::IObject> object);

        void run();

    private:
        void handleEvents();
        void update(sf::Time dt);
        void fixedUpdate(sf::Time dt);

    private:
        sf::Clock clock;
        sf::Time accumulatedTime;

        Renderer renderer;
        std::unique_ptr<::Game::Scene> scene;
    };
} // namespace Engine