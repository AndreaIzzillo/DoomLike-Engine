#pragma once

#include <string>

#include "game/interfaces/imaterial.hpp"
#include "utils/image.hpp"

namespace Game
{
    class TextureMaterial : public IMaterial
    {
    public:
        TextureMaterial(const std::string &filename);

        virtual MaterialDescriptor getDescriptor() const override;
        virtual MaterialSample
        getSample(const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const override;

    private:
        Utils::Image texture;
    };
} // namespace Game
