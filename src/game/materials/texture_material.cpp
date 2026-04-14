#include "game/materials/texture_material.hpp"

namespace Game
{
    TextureMaterial::TextureMaterial(const std::string &filename)
        : texture(filename)
    {}

    MaterialDescriptor TextureMaterial::getDescriptor() const
    {
        MaterialDescriptor descriptor;
        descriptor.isTextured = true;
        descriptor.textureWidth = texture.getWidth();
        descriptor.textureHeight = texture.getHeight();
        return descriptor;
    }

    MaterialSample TextureMaterial::getSample(const HitRecord &record,
                                              const Math::Point2 &texCoord) const
    {
        MaterialSample properties;
        properties.color = texture(texCoord.x, texCoord.y);
        return properties;
    }
} // namespace Game
