#include "game/scene/scene.hpp"
#include <cfloat>

#include "engine/collision_manager.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    Scene::Scene(const Player &player)
        : player(player)
    {}

    Scene::Scene(IO::MapFile &mapFile)
        : player(mapFile.getPlayer())
        , walls(mapFile.getWalls())
    {}

    const Player &Scene::getPlayer() const
    {
        return player;
    }

    const std::vector<std::unique_ptr<IWall>> &Scene::getWalls() const
    {
        return walls;
    }

    const Engine::InputState &Scene::getInputState() const
    {
        return inputState;
    }

    void Scene::setInputState(Engine::InputState inputState)
    {
        this->inputState = inputState;
    }

    void Scene::update(float dt)
    {
        /* Scene update routine: update player and walls based on the current
         * input state and other logic */
        player.update(dt);
        for (const auto &wall : walls)
            wall->update(dt);
    }

    void Scene::fixedUpdate(const Engine::CollisionManager &collisionManager,
                            float dt)
    {
        /* The scene fixed update is responsible for resolving the player's
         * movement intent with collision detection and updating the player and
         * walls accordingly */
        Math::Vector2 velocity = player.computeVelocity(inputState, dt);
        auto [resolvedIntent, hitNormals] =
            collisionManager.resolveVelocity(velocity * dt, player, *this);

        /* Update the player's angular velocity based on the input state */
        player.setAngularVelocity(inputState.rotationDirection);

        /* Scene fixed update routine: update player and walls based on the
         * resolved intent */
        player.fixedUpdate(resolvedIntent / dt, hitNormals, dt);
        collisionManager.pushOut(player, *this);
        player.BobCamera(resolvedIntent.norm() / dt, dt);
        for (const auto &wall : walls)
            wall->fixedUpdate(dt);
    }

    void Scene::addWall(std::unique_ptr<IWall> wall)
    {
        walls.push_back(std::move(wall));
    }
} // namespace Game
