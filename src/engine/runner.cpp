#include "engine/runner.hpp"

#include <optional>

#include "game/settings.hpp"

namespace Engine
{
    Runner::Runner(std::unique_ptr<::Game::Scene> scene)
        : renderer()
        , scene(std::move(scene))
    {}

    void Runner::addWall(std::unique_ptr<::Game::IWall> wall)
    {
        scene->addWall(std::move(wall));
    }

    void Runner::run()
    {
        const sf::Time fixedDt = sf::seconds(
            1.0f / static_cast<float>(::Game::Settings::get().targetFramerate));

        while (renderer.getWindow().isOpen())
        {
            /* First, handle events */
            handleEvents();

            sf::Time dt = clock.restart();

            /* Then, update the scene with the current input state and other
             * logic */
            update(dt);

            accumulatedTime += dt;
            while (accumulatedTime >= fixedDt)
            {
                /* Fixed update for physics and other time-sensitive
                 * calculations */
                fixedUpdate(fixedDt);
                accumulatedTime -= fixedDt;
            }

            /* Finally, render the current state of the scene */
            renderer.render(*scene);

            /* Display the current FPS */
            const auto fps = 1.0f / dt.asSeconds();
            renderer.getWindow().setTitle(
                "Projet ISIM - FPS: " + std::to_string(static_cast<int>(fps)));
        }
    }

    void Runner::handleEvents()
    {
        while (const std::optional event = renderer.getWindow().pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                renderer.getWindow().close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    renderer.getWindow().close();
                }
            }
        }
    }

    void Runner::update(sf::Time dt)
    {
        /* Update the scene with the current input state */
        InputState inputState = inputManager.fetchInputState();
        scene->setInputState(inputState);

        /* Update the scene (calculates physics, logic, etc.) */
        scene->update(dt.asSeconds());
    }

    void Runner::fixedUpdate(sf::Time dt)
    {
        /* Fixed update for physics and other time-sensitive calculations */
        scene->fixedUpdate(collisionManager, dt.asSeconds());
    }
} // namespace Engine
