#include "game/scene/scene.hpp"

#include "engine/collision.hpp"
#include "engine/input.hpp"
#include "game/player/player.hpp"
#include "io/mapfile.hpp"
#include "math/vector2.hpp"

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

    const std::vector<std::unique_ptr<IObject>> &Scene::getObjects() const
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
        player.update(dt);
        for (const auto &object : objects)
            object->update(dt);
    }

    void Scene::fixedUpdate(float dt)
    {
        Math::Vector2 velocity = player.computeVelocity(inputState, dt);
        Engine::CollisionManager collisionManager;
        Math::Vector2 resolvedIntent =
            collisionManager.resolve(velocity * dt, player, *this);

        player.setAngularVelocity(inputState.rotationDirection);

        player.fixedUpdate(resolvedIntent / dt, dt);
        for (const auto &object : objects)
            object->fixedUpdate(dt);
    }

    void Scene::addObject(std::unique_ptr<IObject> object)
    {
        objects.push_back(std::move(object));
    }
} // namespace Game