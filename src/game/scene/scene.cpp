#include "game/scene/scene.hpp"

#include "game/player/player.hpp"
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

    Player &Scene::getPlayer()
    {
        return player;
    }

    const std::vector<std::unique_ptr<IObject>> &Scene::getObjects() const
    {
        return objects;
    }

    void Scene::update(float dt)
    {
        player.update(dt);
        for (const auto &object : objects)
            object->update(dt);
    }

    void Scene::fixedUpdate(float dt)
    {
        player.fixedUpdate(dt);
        for (const auto &object : objects)
            object->fixedUpdate(dt);
    }

    void Scene::addObject(std::unique_ptr<IObject> object)
    {
        objects.push_back(std::move(object));
    }
} // namespace Game