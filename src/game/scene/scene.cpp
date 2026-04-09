#include "game/scene/scene.hpp"

#include "engine/collision_manager.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    Scene::Scene(const Player &player)
        : player(player)
    {}

    Scene::Scene(IO::MapFile &mapFile)
        : player(mapFile.getPlayer())
        , objects(mapFile.getObjects())
    {}

    const Player &Scene::getPlayer() const
    {
        return player;
    }

    const std::vector<std::unique_ptr<IWall>> &Scene::getObjects() const
    {
        return objects;
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
        /* Scene update routine: update player and objects based on the current
         * input state and other logic */
        player.update(dt);
        for (const auto &object : objects)
            object->update(dt);
    }

    void Scene::fixedUpdate(const Engine::CollisionManager &collisionManager,
                            float dt)
    {
        /* The scene fixed update is responsible for resolving the player's
         * movement intent with collision detection and updating the player and
         * objects accordingly */
        Math::Vector2 velocity = player.computeVelocity(inputState, dt);
        Math::Vector2 resolvedIntent =
            collisionManager.resolveVelocity(velocity * dt, player, *this);

        /* Update the player's angular velocity based on the input state */
        player.setAngularVelocity(inputState.rotationDirection);

        /* Scene fixed update routine: update player and objects based on the
         * resolved intent */
        player.fixedUpdate(resolvedIntent / dt, dt);
        for (const auto &object : objects)
            object->fixedUpdate(dt);
    }

    void Scene::addObject(std::unique_ptr<IWall> object)
    {
        objects.push_back(std::move(object));
    }
} // namespace Game
