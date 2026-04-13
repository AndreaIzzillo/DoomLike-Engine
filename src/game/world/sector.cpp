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

    float Sector::getFloorHeight()
    {
        return floorHeight;
    }

    float Sector::getCeilingHeight()
    {
        return ceilingHeight;
    }

    std::shared_ptr<IMaterial> Sector::getFloorMaterial()
    {
        return floorMaterial;
    }

    std::shared_ptr<IMaterial> Sector::getCeilingMaterial()
    {
        return ceilingMaterial;
    }

    const std::vector<Wall *> &Sector::getWalls() const
    {
        return walls;
    }

    void Sector::addWall(Wall *wall)
    {
        walls.push_back(wall);
    }
} // namespace Game
