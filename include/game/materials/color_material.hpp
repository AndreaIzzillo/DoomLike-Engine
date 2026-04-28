#pragma once

#include "game/interfaces/imaterial.hpp"

namespace Game
{
    class ColorMaterial : public IMaterial
    {
    public:
        ColorMaterial(Utils::Color color);

        virtual MaterialDescriptor getDescriptor() const override;
        virtual MaterialSample
        getSample(const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const override;

    private:
        Utils::Color color;
    };
} // namespace Game
