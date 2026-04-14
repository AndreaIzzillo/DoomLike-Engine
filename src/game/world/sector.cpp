#include "game/world/sector.hpp"

namespace Game
{
    Sector::Sector(float floorHeight, float ceilingHeight, std::shared_ptr<IMaterial> floorMaterial,
                   std::shared_ptr<IMaterial> ceilingMaterial)
        : floorHeight(floorHeight)
        , ceilingHeight(ceilingHeight)
        , floorMaterial(std::move(floorMaterial))
        , ceilingMaterial(std::move(ceilingMaterial))
    {}

    float Sector::getFloorHeight() const
    {
        return floorHeight;
    }

    float Sector::getCeilingHeight() const
    {
        return ceilingHeight;
    }

    std::shared_ptr<IMaterial> Sector::getFloorMaterial() const
    {
        return floorMaterial;
    }

    std::shared_ptr<IMaterial> Sector::getCeilingMaterial() const
    {
        return ceilingMaterial;
    }

    const std::vector<Wall *> &Sector::getWalls() const
    {
        return walls;
    }

    TextureTransform Sector::getFloorTextureTransform() const
    {
        return floorTextureTransform;
    }

    TextureTransform Sector::getCeilingTextureTransform() const
    {
        return ceilingTextureTransform;
    }

    void Sector::setFloorTextureTransform(const TextureTransform &transform)
    {
        floorTextureTransform = transform;
    }

    void Sector::setCeilingTextureTransform(const TextureTransform &transform)
    {
        ceilingTextureTransform = transform;
    }

    void Sector::addWall(Wall *wall)
    {
        walls.push_back(wall);
    }
} // namespace Game
