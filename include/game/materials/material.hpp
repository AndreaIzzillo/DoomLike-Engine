#pragma once

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    struct HitRecord;

    /**
     * @brief A structure to hold the properties of a material.
     */
    struct MaterialProperties
    {
        Utils::Color color;
    };

    /**
     * @brief An interface for defining material properties.
     */
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        virtual MaterialProperties getProperties(
            const HitRecord &record,
            const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const = 0;
    };
} // namespace Game