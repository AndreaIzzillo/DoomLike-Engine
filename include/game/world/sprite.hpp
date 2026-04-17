#pragma once

#include <memory>

#include "game/interfaces/imaterial.hpp"
#include "math/point2.hpp"

namespace Game
{
    /**
     * @brief Sprites
     */
    class Sprite
    {
    public:
        Sprite(Math::Point2 pos, std::shared_ptr<IMaterial> material, float mulHeight = 1,
               float mulSize = 1, float vPos = 0);
        ~Sprite() = default;
        const Math::Point2 &getPos();
        const std::shared_ptr<IMaterial> &getMaterial();
        float getMulSize() const;
        float getMulHeight() const;
        float getVPos() const;

    private:
        Math::Point2 pos;
        std::shared_ptr<Game::IMaterial> material;
        float mulSize;
        float mulHeight;
        float vPos;
    };
} // namespace Game
