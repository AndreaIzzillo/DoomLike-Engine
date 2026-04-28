#pragma once

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    struct HitRecord;

    struct MaterialDescriptor
    {
        bool isTextured = false;

        int textureWidth = 0;
        int textureHeight = 0;
    };

    struct MaterialSample
    {
        Utils::Color color = Utils::Color(1.f, 1.f, 1.f);
    };

    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        virtual MaterialDescriptor getDescriptor() const = 0;

        virtual MaterialSample
        getSample(const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const = 0;
    };
} // namespace Game
