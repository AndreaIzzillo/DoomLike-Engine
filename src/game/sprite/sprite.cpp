#include "game/sprite/sprite.hpp"

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    Sprite::Sprite(Math::Point2 pos, const std::string &filename)
        : pos(pos)
        , texture(Utils::Image(filename))
    {}

    const Math::Point2 &Sprite::getPos()
    {
        return pos;
    }

    const Utils::Image &Sprite::getTexture()
    {
        return texture;
    }

}; // namespace Game