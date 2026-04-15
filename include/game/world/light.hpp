#pragma once

#include "math/point2.hpp"
#include "utils/image.hpp"

namespace Game
{
    class Light
    {
    public:
        Light(const Math::Point2 &position, float radius, float intensity,
              const Utils::Color &color);

        float getIntensityAt(const Math::Point2 &point) const;
        Utils::Color getColor() const;

    private:
        Math::Point2 position;
        float radius;

        Utils::Color color;
        float intensity;
    };
} // namespace Game