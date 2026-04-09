#pragma once

#include <memory>
#include <vector>

#include "engine/input.hpp"
#include "game/objects/object.hpp"
#include "game/player/player.hpp"
#include "io/mapfile.hpp"

namespace Game
{
    /**
     * @brief A class representing the game scene.
     */
    class Scene
    {
    private:
        Player player;
        std::vector<std::unique_ptr<IObject>> objects;
        Engine::InputState inputState;

    public:
        Scene(const Player &player);
        Scene(IO::MapFile &mapFile);

        ~Scene() = default;

        /* Getters */
        const Player &getPlayer() const;
        const Engine::InputState &getInputState() const;

        const std::vector<std::unique_ptr<IObject>> &getObjects() const;

        void update(float dt);
        void fixedUpdate(float dt);

        /* Object management */
        void addObject(std::unique_ptr<IObject> object);

        /* Input state management */
        void setInputState(Engine::InputState inputState);
    };
} // namespace Game