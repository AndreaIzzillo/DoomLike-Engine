#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "game/interfaces/imaterial.hpp"
#include "game/player/player.hpp"
#include "game/sprite/sprite.hpp"
#include "game/world/sector.hpp"
#include "game/world/wall.hpp"
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
        std::vector<std::unique_ptr<Wall>> getWalls();
        std::vector<std::unique_ptr<Sector>> getSectors();
        std::vector<std::unique_ptr<Sprite>> getSprites();
        const Sector *getStartingSector() const;

    private:
        Point2 playerPosition;
        Point2 playerLookAt;
        std::vector<std::unique_ptr<Wall>> walls;
        std::vector<std::unique_ptr<Sprite>> sprites;
        std::map<std::string, std::shared_ptr<IMaterial>> materials;
        std::map<int, std::unique_ptr<Sector>> sectors;
    };
} // namespace IO
