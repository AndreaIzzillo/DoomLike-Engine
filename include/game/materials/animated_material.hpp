#pragma once

#include "game/interfaces/imaterial.hpp"
#include "utils/image.hpp"
namespace Game
{
    class AnimatedMaterial : public IMaterial
    {
    public:
        AnimatedMaterial(int width, int height);
        AnimatedMaterial(const Utils::AnimatedImage &animatedImage);

        void update(float dt);

        virtual MaterialDescriptor getDescriptor() const override;
        virtual MaterialSample
        getSample(const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const override;

        void addImageFrame(const Utils::Image &image, float durationMs);

    private:
        Utils::AnimatedImage animatedImage;
    };
} // namespace Game