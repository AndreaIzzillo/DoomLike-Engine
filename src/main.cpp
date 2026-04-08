#include "engine/runner.hpp"
#include "game/player.hpp"
#include "game/scene.hpp"
#include "game/wall.hpp"
#include "io/mapfile.hpp"

int main()
{
    auto mapfile = IO::MapFile("resources/scene_easy.map");
    auto scene = std::make_unique<Game::Scene>(mapfile);

    auto runner = std::make_unique<Engine::Runner>(std::move(scene));
    runner->run();
    return 0;
}