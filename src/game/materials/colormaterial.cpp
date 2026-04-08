#include "game/materials/colormaterial.hpp"

namespace Game
{
    ColorMaterial::ColorMaterial(Utils::Color color)
        : color(color)
    {}

    MaterialProperties
    ColorMaterial::getProperties(const HitRecord &record,
                                 const Math::Point2 &texCoord) const
    {
        MaterialProperties properties;
        properties.color = color;
        return properties;
    }
} // namespace Game
