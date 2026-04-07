#include "engine/runner.hpp"

#include "game/settings.hpp"

namespace Engine
{
    Runner::Runner()
        : renderer()
        , scene(std::make_unique<::Game::Scene>())
    {}

    Runner::Runner(std::unique_ptr<::Game::Scene> scene)
        : renderer()
        , scene(std::move(scene))
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
        }
    }

    void Runner::handleEvents()
    {
        // TODO: Input handling
    }

    void Runner::update(sf::Time dt)
    {
        scene->update(dt.asSeconds());
    }

    void Runner::fixedUpdate(sf::Time dt)
    {
        scene->fixedUpdate(dt.asSeconds());
    }
} // namespace Engine