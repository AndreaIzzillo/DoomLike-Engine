#include "game/sprite/sprite.hpp"

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    Sprite::Sprite(Math::Point2 pos, std::shared_ptr<IMaterial> material, float mulHeight,
                   float mulSize, float vPos)
        : pos(pos)
        , material(material)
        , mulHeight(mulHeight)
        , mulSize(mulSize)
        , vPos(vPos)
    {}

    const Math::Point2 &Sprite::getPos()
    {
        return pos;
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

    const std::shared_ptr<IMaterial> &Sprite::getMaterial()
    {
        return material;
    }

}; // namespace Game