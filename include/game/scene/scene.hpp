#pragma once

#include <memory>
#include <vector>

#include "engine/input_manager.hpp"
#include "game/player/player.hpp"
#include "game/world/sector.hpp"
#include "game/world/wall.hpp"

/* Forward declaration to avoid circular dependency */
namespace Engine
{
    class CollisionManager;
}

namespace IO
{
    class MapFile;
}

namespace Game
{
    /**
     * @brief Runtime container for player, walls, and last input state.
     *
     * Scene applies per-frame updates, then fixed-step simulation where player
     * velocity is resolved through CollisionManager before physics are applied.
     */
    class Scene
    {
    private:
        Player player;
        const Sector *currentSector;

        std::vector<std::unique_ptr<Sector>> sectors;
        std::vector<std::unique_ptr<Wall>> walls;

        Engine::InputState inputState;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        const Engine::InputState &getInputState() const;

        const std::vector<std::unique_ptr<Wall>> &getWalls() const;
        const std::vector<std::unique_ptr<Sector>> &getSectors() const;

        const Sector *getCurrentSector() const;

        void update(float dt);
        void fixedUpdate(const Engine::CollisionManager &collisionManager,
                         float dt);

        /* World management */
        void addSector(std::unique_ptr<Sector> sector);
        void addWall(std::unique_ptr<Wall> wall);
        void setCurrentSector(const Sector *sector);

        /* Input state management */
        void setInputState(Engine::InputState inputState);
    };
} // namespace Game
