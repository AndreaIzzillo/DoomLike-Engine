#include <memory>

#include "engine/runner.hpp"
#include "game/scene/scene.hpp"
#include "game/world/light.hpp"
#include "io/mapfile.hpp"

int main()
{
    auto mapfile = IO::MapFile("resources/maps/test_stairs.map");
    auto scene = std::make_unique<Game::Scene>(mapfile);
    scene->addLight(std::make_unique<Game::Light>(Math::Point2(4.f, 4.f), 7.f, 1.f, CYAN));
    scene->addLight(std::make_unique<Game::Light>(Math::Point2(4.f, -4.f), 7.f, 1.f, YELLOW));
    scene->addLight(std::make_unique<Game::Light>(Math::Point2(-4.f, 4.f), 7.f, 1.f, MAGENTA));
    scene->addLight(std::make_unique<Game::Light>(Math::Point2(-4.f, -4.f), 7.f, 1.f, RED));
    scene->addLight(std::make_unique<Game::Light>(Math::Point2(19.f, 0.f), 7.f, 1.5f, YELLOW));
    auto runner = std::make_unique<Engine::Runner>(std::move(scene));

    runner->run();
    return 0;
}