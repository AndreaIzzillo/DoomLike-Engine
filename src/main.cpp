#include "engine/runner.hpp"
#include "game/player.hpp"
#include "game/scene.hpp"
#include "game/wall.hpp"

int main()
{
    auto player = Game::Player({ 0.0f, 0.0f }, { 1.0f, 0.0f });
    auto scene = std::make_unique<Game::Scene>(player);
    scene->addObject(std::make_unique<Game::Wall>(
        Math::Point2(3.0f, 0.1f), Math::Point2(2.0f, -0.1f), 200.0f));
    auto runner = std::make_unique<Engine::Runner>(std::move(scene));
    runner->run();
    return 0;
}