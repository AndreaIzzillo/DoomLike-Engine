#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "game/materials/material.hpp"
#include "game/objects/object.hpp"
#include "game/player/player.hpp"
#include "math/point2.hpp"

namespace IO
{
    using namespace Game;
    using namespace Math;

    /**
     * @brief A class representing a map file.
     *
     * This class is responsible for loading a map file and extracting the
     * player position, lookAt and the objects in the scene.
     * Grammar of the map file defined in resources/maps/GRAMMAR.md
     */
    class MapFile
    {
    public:
        MapFile() = default;
        MapFile(const std::string &path);
        ~MapFile() = default;

        Player getPlayer() const;
        std::vector<std::unique_ptr<IWall>> getObjects();

    private:
        Point2 playerPosition;
        Point2 playerLookAt;
        std::vector<std::unique_ptr<IWall>> objects;
        std::map<std::string, std::shared_ptr<IMaterial>> materials;
    };
} // namespace IO