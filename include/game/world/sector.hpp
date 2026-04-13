#pragma once

#include <vector>

#include "game/interfaces/imaterial.hpp"
#include "game/world/wall.hpp"

namespace Game
{
    class Sector
    {
    public:
        Sector(float floorHeight, float ceilingHeight,
               std::shared_ptr<IMaterial> floorMaterial,
               std::shared_ptr<IMaterial> ceilingMaterial);
        ~Sector() = default;

        /* Getters */
        float getFloorHeight();
        float getCeilingHeight();
        std::shared_ptr<IMaterial> getFloorMaterial();
        std::shared_ptr<IMaterial> getCeilingMaterial();
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
