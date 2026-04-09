#pragma once

#include <string>

#include "game/interfaces/imaterial.hpp"
#include "utils/image.hpp"

namespace Game
{
    /**
     * @brief A class representing a texture material.
     *
     * Texture are images that can be mapped onto the surface of a 3D object.
     * File formats supported is ASCII PPM.
     */
    class TextureMaterial : public IMaterial
    {
    public:
        TextureMaterial(const std::string &filename);

        virtual MaterialDescriptor getDescriptor() const override;

        virtual MaterialSample getSample(
            const HitRecord &record,
            const Math::Point2 &texCoord = Math::Point2(0.0f,
                                                        0.0f)) const override;

    private:
        Utils::Image texture;
    };
} // namespace Game
