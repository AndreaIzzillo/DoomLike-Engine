#pragma once

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    struct HitRecord;

    /**
     * @brief A structure to hold descriptor information for a material.
     */
    struct MaterialDescriptor
    {
        bool isTextured = false;

        int textureWidth = 0;
        int textureHeight = 0;
    };

    /**
     * @brief A structure to hold material properties at a hit point.
     */
    struct MaterialSample
    {
        Utils::Color color = Utils::Color(1.f, 1.f, 1.f);
    };

    /**
     * @brief An interface for defining material properties.
     */
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        virtual MaterialDescriptor getDescriptor() const = 0;

        virtual MaterialSample getSample(
            const HitRecord &record,
            const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const = 0;
    };
} // namespace Game