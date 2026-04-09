#pragma once

#include <memory>
#include <vector>

#include "engine/input_manager.hpp"
#include "game/interfaces/iwall.hpp"
#include "game/player/player.hpp"

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
        std::vector<std::unique_ptr<IWall>> walls;

        Engine::InputState inputState;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        const Engine::InputState &getInputState() const;

        const std::vector<std::unique_ptr<IWall>> &getWalls() const;

        void update(float dt);
        void fixedUpdate(const Engine::CollisionManager &collisionManager,
                         float dt);

        /* Object management */
        void addObject(std::unique_ptr<IWall> object);

        /* Input state management */
        void setInputState(Engine::InputState inputState);
    };
} // namespace Game
