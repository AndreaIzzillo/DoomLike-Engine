#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "game/interfaces/imaterial.hpp"
#include "game/interfaces/iwall.hpp"
#include "game/player/player.hpp"
#include "math/point2.hpp"

namespace IO
{
    using namespace Game;
    using namespace Math;

    /**
     * @brief Loads a map description and builds the initial scene content.
     *
     * It parses player spawn, material declarations, and wall definitions so
     * Scene can be constructed from file data at startup.
     */
    class MapFile
    {
    public:
        MapFile() = default;
        MapFile(const std::string &path);
        ~MapFile() = default;

        Player getPlayer() const;
        std::vector<std::unique_ptr<IWall>> getWalls();

    private:
        Point2 playerPosition;
        Point2 playerLookAt;
        std::vector<std::unique_ptr<IWall>> walls;
        std::map<std::string, std::shared_ptr<IMaterial>> materials;
    };
} // namespace IO
