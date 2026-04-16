#pragma once

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    struct HitRecord;

    /**
     * @brief Static material metadata used by the renderer.
     *
     * Describes whether sampling is texture-based and, when textured, exposes
     * texture dimensions for coordinate mapping.
     */
    struct MaterialDescriptor
    {
        bool isTextured = false;

        int textureWidth = 0;
        int textureHeight = 0;
    };

    /**
     * @brief Result of a material sample at a specific hit location.
     *
     * Currently stores the final surface color used for a rendered pixel.
     */
    struct MaterialSample
    {
        Utils::Color color = Utils::Color(1.f, 1.f, 1.f);
    };

    /**
     * @brief Rendering material interface sampled after a ray hit.
     *
     * Implementations expose texture metadata and return the surface color used
     * by Renderer when drawing wall columns.
     */
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        virtual MaterialDescriptor getDescriptor() const = 0;

        virtual MaterialSample
        getSample(const Math::Point2 &texCoord = Math::Point2(0.0f, 0.0f)) const = 0;
    };
} // namespace Game
