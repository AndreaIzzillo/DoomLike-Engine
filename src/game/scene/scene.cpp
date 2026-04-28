#include "game/scene/scene.hpp"

#include <cfloat>
#include <memory>

#include "engine/collision_manager.hpp"
#include "game/world/sprite.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    Scene::Scene(const Player &player)
        : player(player)
    {}

    Scene::Scene(IO::MapFile &mapFile)
        : player(mapFile.getPlayer())
        , walls(mapFile.getWalls())
        , sectors(mapFile.getSectors())
        , currentSector(mapFile.getStartingSector())
        , sprites(mapFile.getSprites())
        , lights(mapFile.getLights())
    {
        player.setCurrentSector(currentSector);

        for (auto &animMat : mapFile.getAnimatedMaterials())
        {
            animationManager.addAnimatedMaterial(animMat);
        }
    }

    const Player &Scene::getPlayer() const
    {
        return player;
    }

    const std::vector<std::unique_ptr<Sector>> &Scene::getSectors() const
    {
        return sectors;
    }

    const std::vector<std::unique_ptr<Wall>> &Scene::getWalls() const
    {
        return walls;
    }

    const std::vector<std::unique_ptr<Light>> &Scene::getLights() const
    {
        return lights;
    }

    const Engine::InputState &Scene::getInputState() const
    {
        return inputState;
    }

    const Engine::AnimationManager &Scene::getAnimationManager() const
    {
        return animationManager;
    }

    const Sector *Scene::getCurrentSector() const
    {
        return currentSector;
    }

    const std::vector<std::unique_ptr<Sprite>> &Scene::getSprites() const
    {
        return sprites;
    }

    void Scene::setInputState(Engine::InputState inputState)
    {
        this->inputState = inputState;
    }

    void Scene::update(float dt)
    {
        animationManager.update(dt);
        player.update(dt);
    }

    void Scene::fixedUpdate(const Engine::CollisionManager &collisionManager, float dt)
    {
        /* Compute the player's velocity based on the input state */
        Math::Vector2 velocity = player.computeVelocity(inputState, dt);
        auto [resolvedIntent, newSector] =
            collisionManager.computeCollision(velocity * dt, player, *this);

        /* If the player has entered a new sector, update the current sector */
        if (newSector)
        {
            setCurrentSector(newSector);
        }

        /* Update the player's angular velocity based on the input state */
        player.setAngularVelocity(inputState.rotationDirection);
        player.fixedUpdate(resolvedIntent / dt, dt);
    }

    void Scene::addWall(std::unique_ptr<Wall> wall)
    {
        walls.push_back(std::move(wall));
    }

    void Scene::addSector(std::unique_ptr<Sector> sector)
    {
        sectors.push_back(std::move(sector));
    }

    void Scene::addLight(std::unique_ptr<Light> light)
    {
        lights.push_back(std::move(light));
    }

    void Scene::addSprite(std::unique_ptr<Sprite> sprite)
    {
        sprites.push_back(std::move(sprite));
    }

    void Scene::setCurrentSector(const Sector *sector)
    {
        currentSector = sector;
        player.setCurrentSector(sector);
    }
} // namespace Game
