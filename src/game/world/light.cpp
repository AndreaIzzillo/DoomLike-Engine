#include "game/world/light.hpp"

namespace Game
{
    Light::Light(const Math::Point2 &position, float radius, float intensity,
                 const Utils::Color &color)
        : position(position)
        , radius(radius)
        , color(color)
        , intensity(intensity)
    {}

    float Light::getIntensityAt(const Math::Point2 &point) const
    {
        auto toPoint = point - position;
        float distance = toPoint.norm();
        if (distance > radius)
            return 0.f;

        return intensity * (1.f - distance / radius);
    }

    Utils::Color Light::getColor() const
    {
        return color;
    }
} // namespace Game