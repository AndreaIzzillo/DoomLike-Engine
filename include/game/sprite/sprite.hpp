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
        Sprite(Math::Point2 pos, const std::string &filename);
        ~Sprite() = default;
        const Math::Point2 &getPos();
        const Utils::Image &getTexture();

    private:
        Math::Point2 pos;
        Utils::Image texture;
    };
} // namespace Game
