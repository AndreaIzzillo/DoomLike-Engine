#include "game/scene.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "game/enemy.hpp"
#include "game/segment.hpp"
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

    const Player &Scene::get_player() const
    {
        return player;
    }

    const std::vector<std::unique_ptr<IObject>> &Scene::get_objects() const
    {
        return objects;
    }

    void Scene::update(float dt)
    {
        player.update(dt);
        for (const auto &object : objects)
            object->update(dt);
    }

    void Scene::add_object(std::unique_ptr<IObject> object)
    {
        objects.push_back(std::move(object));
    }
} // namespace Game