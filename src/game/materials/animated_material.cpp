#include "game/materials/animated_material.hpp"

#include "game/interfaces/imaterial.hpp"

namespace Game
{
    AnimatedMaterial::AnimatedMaterial(int width, int height)
        : animatedImage(width, height)
    {}

    AnimatedMaterial::AnimatedMaterial(const Utils::AnimatedImage &animatedImage)
        : animatedImage(animatedImage)
    {}

    void AnimatedMaterial::update(float dt)
    {
        animatedImage.updateScheduler(dt);
    }

    MaterialDescriptor AnimatedMaterial::getDescriptor() const
    {
        MaterialDescriptor descriptor;
        descriptor.isTextured = true;
        descriptor.textureWidth = animatedImage.getWidth();
        descriptor.textureHeight = animatedImage.getHeight();
        return descriptor;
    }

    MaterialSample AnimatedMaterial::getSample(const Math::Point2 &texCoord) const
    {
        MaterialSample properties;
        properties.color = animatedImage(texCoord.x, texCoord.y);
        return properties;
    }

    void AnimatedMaterial::addImageFrame(const Utils::Image &image, float durationMs)
    {
        animatedImage.addImageFrame(image, durationMs);
    }
} // namespace Game
