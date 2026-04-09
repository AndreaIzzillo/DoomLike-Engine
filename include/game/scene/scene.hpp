#pragma once

#include <memory>
#include <vector>

#include "game/objects/object.hpp"
#include "game/player/player.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    /**
     * @brief A class representing the game scene.
     */
    class Scene
    {
    private:
        Player player;
        std::vector<std::unique_ptr<IWall>> objects;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        Player &getPlayer(); // Non-const version for player movement. (used in
                             // InputManager.hpp)
        const std::vector<std::unique_ptr<IWall>> &getObjects() const;

        void update(float dt);
        void fixedUpdate(float dt);

        /* Object management */
        void addObject(std::unique_ptr<IWall> object);
    };
} // namespace Game