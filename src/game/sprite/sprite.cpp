#include "game/sprite/sprite.hpp"

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    Sprite::Sprite(Math::Point2 pos, const std::string &filename, float mulHeight, float mulSize,
                   float vPos)
        : pos(pos)
        , texture(Utils::Image(filename))
        , mulHeight(mulHeight)
        , mulSize(mulSize)
        , vPos(vPos)
    {}

    const Math::Point2 &Sprite::getPos()
    {
        return pos;
    }

    const Utils::Image &Sprite::getTexture()
    {
        return texture;
    }

    float Sprite::getMulSize() const
    {
        return mulSize;
    }
    float Sprite::getMulHeight() const
    {
        return mulHeight;
    }
    float Sprite::getVPos() const
    {
        return vPos;
    }

}; // namespace Game