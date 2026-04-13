#pragma once

#include <vector>

#include "game/interfaces/imaterial.hpp"
#include "game/world/wall.hpp"

namespace Game
{
    /**
     * @brief A sector in the game world, representing a 3D space with a floor
     * and ceiling.
     *
     * A sector is defined by its floor and ceiling heights, materials, and the
     * walls that enclose it. It serves as a container for walls and provides
     * context for rendering and collision detection. Sectors can be connected
     * by portal walls, allowing the player to see and
     */
    class Sector
    {
    public:
        Sector(float floorHeight, float ceilingHeight,
               std::shared_ptr<IMaterial> floorMaterial,
               std::shared_ptr<IMaterial> ceilingMaterial);
        ~Sector() = default;

        /* Getters */
        float getFloorHeight() const;
        float getCeilingHeight() const;
        std::shared_ptr<IMaterial> getFloorMaterial() const;
        std::shared_ptr<IMaterial> getCeilingMaterial() const;
        const std::vector<Wall *> &getWalls() const;

        void addWall(Wall *wall);

    private:
        std::vector<Wall *> walls;

        float floorHeight;
        float ceilingHeight;

        std::shared_ptr<IMaterial> floorMaterial;
        std::shared_ptr<IMaterial> ceilingMaterial;
    };
} // namespace Game
