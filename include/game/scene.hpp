#pragma once

#include <memory>
#include <vector>

#include "game/object.hpp"
#include "game/player.hpp"

namespace Game
{
    class Scene
    {
    private:
        std::unique_ptr<Player> player;
        std::vector<std::unique_ptr<IObject>> objects;

    public:
        Scene(std::unique_ptr<Player> player);
        Scene(const std::string &path);
        Scene() = default;

        ~Scene() = default;

        const Player &get_player() const;
        const std::vector<std::unique_ptr<IObject>> &get_objects() const;

        void update(float dt);

        void add_object(std::unique_ptr<IObject> object);
    };
} // namespace Game