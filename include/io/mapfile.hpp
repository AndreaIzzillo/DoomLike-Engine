#pragma once

#include <memory>
#include <string>
#include <vector>

#include "game/object.hpp"
#include "game/player.hpp"
#include "math/point2.hpp"

namespace IO
{
    using namespace Game;
    using namespace Math;

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