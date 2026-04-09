#pragma once

#include <memory>
#include <vector>

#include "engine/input.hpp"
#include "game/objects/object.hpp"
#include "game/player/player.hpp"
#include "io/mapfile.hpp"

/* Forward declaration to avoid circular dependency */
namespace Engine
{
    class CollisionManager;
}

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

        Engine::InputState inputState;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        const Engine::InputState &getInputState() const;

        const std::vector<std::unique_ptr<IWall>> &getObjects() const;

        void update(float dt);
        void fixedUpdate(const Engine::CollisionManager &collisionManager,
                         float dt);

        /* Object management */
        void addObject(std::unique_ptr<IWall> object);

        /* Input state management */
        void setInputState(Engine::InputState inputState);
    };
} // namespace Game