#pragma once

#include <memory>
#include <vector>

#include "game/object.hpp"
#include "game/player.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    class Scene
    {
    private:
        Player player;
        std::vector<std::unique_ptr<IObject>> objects;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);
        Scene() = default;

        ~Scene() = default;

        const Player &getPlayer() const;
        const std::vector<std::unique_ptr<IObject>> &getObjects() const;

        void update(float dt);
        void fixedUpdate(float dt);

        void addObject(std::unique_ptr<IObject> object);
    };
} // namespace Game