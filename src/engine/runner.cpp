#include "engine/runner.hpp"

#include <optional>

#include "game/settings.hpp"

namespace Engine
{
    Runner::Runner(std::unique_ptr<::Game::Scene> scene)
        : renderer()
        , scene(std::move(scene))
        , inputManager(scene.get())
    {}

    void Runner::addObject(std::unique_ptr<::Game::IObject> object)
    {
        scene->addObject(std::move(object));
    }

    void Runner::run()
    {
        const sf::Time fixedDt = sf::seconds(
            1.0f / static_cast<float>(::Game::Settings::get().targetFramerate));

        while (renderer.getWindow().isOpen())
        {
            handleEvents();

            sf::Time dt = clock.restart();
            accumulatedTime += dt;

            while (accumulatedTime >= fixedDt)
            {
                fixedUpdate(fixedDt);
                accumulatedTime -= fixedDt;
            }

            update(dt);
            renderer.render(*scene);

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
        inputManager.update();
        scene->update(dt.asSeconds());
    }

    void Runner::fixedUpdate(sf::Time dt)
    {
        scene->fixedUpdate(dt.asSeconds());
    }
} // namespace Engine
