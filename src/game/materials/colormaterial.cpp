#include "game/materials/colormaterial.hpp"

#include "game/materials/material.hpp"

namespace Game
{
    ColorMaterial::ColorMaterial(Utils::Color color)
        : color(color)
    {}

    MaterialDescriptor ColorMaterial::getDescriptor() const
    {
        MaterialDescriptor descriptor;
        descriptor.isTextured = false;
        return descriptor;
    }

    MaterialSample ColorMaterial::getSample(const HitRecord &record,
                                            const Math::Point2 &texCoord) const
    {
        MaterialSample properties;
        properties.color = color;
        return properties;
    }
} // namespace Game
