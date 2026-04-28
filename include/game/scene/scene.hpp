#pragma once

#include <memory>
#include <vector>

#include "engine/animation_manager.hpp"
#include "engine/input_manager.hpp"
#include "game/player/player.hpp"
#include "game/world/sprite.hpp"
#include "game/world/light.hpp"
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
    class Scene
    {
    private:
        Player player;
        const Sector *currentSector;

        std::vector<std::unique_ptr<Sector>> sectors;
        std::vector<std::unique_ptr<Light>> lights;
        std::vector<std::unique_ptr<Sprite>> sprites;

        Engine::InputState inputState;
        Engine::AnimationManager animationManager;
        std::vector<std::unique_ptr<Wall>> walls;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        const Engine::InputState &getInputState() const;
        const Engine::AnimationManager &getAnimationManager() const;

        const std::vector<std::unique_ptr<Wall>> &getWalls() const;
        const std::vector<std::unique_ptr<Sector>> &getSectors() const;
        const std::vector<std::unique_ptr<Light>> &getLights() const;
        const std::vector<std::unique_ptr<Sprite>> &getSprites() const;

        const Sector *getCurrentSector() const;

        void update(float dt);
        void fixedUpdate(const Engine::CollisionManager &collisionManager, float dt);

        /* World management */
        void addSector(std::unique_ptr<Sector> sector);
        void addWall(std::unique_ptr<Wall> wall);
        void addLight(std::unique_ptr<Light> light);
        void addSprite(std::unique_ptr<Sprite> sprite);
        void setCurrentSector(const Sector *sector);

        /* Input state management */
        void setInputState(Engine::InputState inputState);
    };
} // namespace Game
