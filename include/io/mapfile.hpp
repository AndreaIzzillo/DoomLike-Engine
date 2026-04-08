#pragma once

#include <memory>
#include <string>
#include <vector>

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
     * player position, lookAt and the objects in the scene. Grammar of the map
     * file is as follows:
     * - Player (should be unique): "P (x, y) (lookAtX, lookAtY)"
     * - Wall: "W (startX, startY) (endX, endY) height"
     * - Enemy: "E (x, y)"
     */
    class MapFile
    {
    public:
        MapFile() = default;
        MapFile(const std::string &path);
        ~MapFile() = default;

        Player getPlayer() const;
        std::vector<std::unique_ptr<IObject>> getObjects();

    private:
        Point2 playerPosition;
        Point2 playerLookAt;
        std::vector<std::unique_ptr<IObject>> objects;
    };
} // namespace IO