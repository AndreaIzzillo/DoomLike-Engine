#include "engine/runner.hpp"

#include <optional>

#include "game/settings.hpp"
#include "game/sprite/sprite.hpp"

namespace Engine
{
    Runner::Runner(std::unique_ptr<::Game::Scene> scene)
        : renderer()
        , scene(std::move(scene))
    {
        if (this->scene == nullptr)
            throw std::invalid_argument("Scene cannot be null");
        if (this->scene->getCurrentSector() == nullptr)
            throw std::invalid_argument("Scene must have a valid current sector");
    }

    void Runner::addWall(std::unique_ptr<::Game::Wall> wall)
    {
        scene->addWall(std::move(wall));
    }

    void Runner::addSprite(std::unique_ptr<::Game::Sprite> sprite)
    {
        scene->addSprite(std::move(sprite));
    }

    void Runner::run()
    {
        int fixedUpdateFrequency = std::max(60, Game::Settings::get().targetFramerate);
        const sf::Time fixedDt = sf::seconds(1.f / fixedUpdateFrequency);

        int fpsCounter = 0;
        float fps = 0.f;
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
            fps += 1.0f / dt.asSeconds();
            if (fpsCounter == 15)
            {
                float averageFps = fps / static_cast<float>(fpsCounter);
                renderer.getWindow().setTitle("Projet ISIM - FPS: "
                                              + std::to_string(static_cast<int>(averageFps)));
                fpsCounter = 0;
                fps = 0.f;
            }
            fpsCounter++;
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

                if (keyPressed->scancode == sf::Keyboard::Scancode::P)
                {
                    std::cout << scene->getPlayer().getPosition().x << ", "
                              << scene->getPlayer().getPosition().y << std::endl;
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
