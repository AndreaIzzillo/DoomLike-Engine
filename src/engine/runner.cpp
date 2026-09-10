#include "engine/runner.hpp"

#include <SFML/Audio.hpp>
#include <optional>

#include "game/settings.hpp"
#include "game/world/sprite.hpp"

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
        sf::Music music("resources/audio/E1M1.mp3");
        music.setLooping(true);
        music.play();

        int fixedUpdateFrequency = std::max(60, Game::Settings::get().targetFramerate * 16);
        const sf::Time fixedDt = sf::seconds(1.f / fixedUpdateFrequency);

        int fpsCounter = 0;
        float fps = 0.f;
        while (renderer.getWindow().isOpen())
        {
            handleEvents();

            sf::Time dt = clock.restart();

            update(dt);

            accumulatedTime += dt;
            while (accumulatedTime >= fixedDt)
            {
                fixedUpdate(fixedDt);
                accumulatedTime -= fixedDt;
            }

            renderer.render(*scene);

            fps += 1.0f / dt.asSeconds();
            if (fpsCounter == 10)
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
        InputState inputState = inputManager.fetchInputState();
        scene->setInputState(inputState);

        scene->update(dt.asSeconds());
    }

    void Runner::fixedUpdate(sf::Time dt)
    {
        scene->fixedUpdate(collisionManager, dt.asSeconds());
    }
} // namespace Engine
