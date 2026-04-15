#pragma once

#include <string>

#include "math/point2.hpp"
#include "utils/image.hpp"
namespace Game
{
    /**
     * @brief Sprites
     */
    class Sprite
    {
    public:
        Sprite(Math::Point2 pos, const std::string &filename, float mulHeight = 1,
               float mulSize = 1, float vPos = 0);
        ~Sprite() = default;
        const Math::Point2 &getPos();
        const Utils::Image &getTexture();
        float getMulSize() const;
        float getMulHeight() const;
        float getVPos() const;

    private:
        Math::Point2 pos;
        Utils::Image texture;
        float mulSize;
        float mulHeight;
        float vPos;
    };
} // namespace Game
