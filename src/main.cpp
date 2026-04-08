#include "engine/runner.hpp"
#include "game/scene/scene.hpp"
#include "io/mapfile.hpp"

int main()
{
    auto mapfile = IO::MapFile("resources/maps/test.map");
    auto scene = std::make_unique<Game::Scene>(mapfile);
    auto runner = std::make_unique<Engine::Runner>(std::move(scene));

    runner->run();
    return 0;
}