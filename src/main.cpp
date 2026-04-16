#include <memory>
#include <string>

#include "engine/runner.hpp"
#include "game/scene/scene.hpp"
#include "io/mapfile.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <mapfile>" << std::endl;
        return 1;
    }

    std::string mapFilePath = "resources/maps/" + std::string(argv[1]);

    auto mapfile = IO::MapFile(mapFilePath);
    auto scene = std::make_unique<Game::Scene>(mapfile);
    auto runner = std::make_unique<Engine::Runner>(std::move(scene));

    runner->run();
    return 0;
}