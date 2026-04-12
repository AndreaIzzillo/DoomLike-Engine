#include "game/world/sector.hpp"

namespace Game
{
    Sector::Sector(float floorHeight, float ceilingHeight,
                   std::shared_ptr<IMaterial> floorMaterial,
                   std::shared_ptr<IMaterial> ceilingMaterial)
        : floorHeight(floorHeight)
        , ceilingHeight(ceilingHeight)
        , floorMaterial(std::move(floorMaterial))
        , ceilingMaterial(std::move(ceilingMaterial))
    {}

    void Sector::addWall(Wall *wall)
    {
        walls.push_back(wall);
    }
} // namespace Game
