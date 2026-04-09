#pragma once

#include "game/interfaces/imaterial.hpp"

namespace Game
{
    /**
     * @brief A simple material that returns a constant color regardless of the
     * texture coordinates.
     *
     * This material can be used for objects that do not require texture mapping
     * and should be rendered with a single color.
     */
    class ColorMaterial : public IMaterial
    {
    public:
        ColorMaterial(Utils::Color color);

        virtual MaterialDescriptor getDescriptor() const override;

        virtual MaterialSample getSample(
            const HitRecord &record,
            const Math::Point2 &texCoord = Math::Point2(0.0f,
                                                        0.0f)) const override;

    private:
        Utils::Color color;
    };
} // namespace Game
