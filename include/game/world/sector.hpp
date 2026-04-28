#pragma once

#include <vector>

#include "game/interfaces/imaterial.hpp"
#include "game/world/sprite.hpp"
#include "game/world/wall.hpp"

namespace Game
{
    class Sector
    {
    public:
        Sector(float floorHeight, float ceilingHeight, std::shared_ptr<IMaterial> floorMaterial,
               std::shared_ptr<IMaterial> ceilingMaterial);
        ~Sector() = default;

        /* Getters */
        float getFloorHeight() const;
        float getCeilingHeight() const;
        std::shared_ptr<IMaterial> getFloorMaterial() const;
        std::shared_ptr<IMaterial> getCeilingMaterial() const;
        const std::vector<Wall *> &getWalls() const;
        TextureTransform getFloorTextureTransform() const;
        TextureTransform getCeilingTextureTransform() const;
        const std::vector<Sprite *> getSprites() const;

        void setFloorTextureTransform(const TextureTransform &transform);
        void setCeilingTextureTransform(const TextureTransform &transform);

        void addWall(Wall *wall);
        void addSprite(Sprite *sprite);

    private:
        std::vector<Wall *> walls;
        std::vector<Sprite *> sprites;

        float floorHeight;
        float ceilingHeight;

        std::shared_ptr<IMaterial> floorMaterial;
        std::shared_ptr<IMaterial> ceilingMaterial;

        TextureTransform floorTextureTransform;
        TextureTransform ceilingTextureTransform;
    };
} // namespace Game
