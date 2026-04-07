#pragma once

#include "game/scene.hpp"
#include "math/ray.hpp"
#include "utils/image.hpp"

namespace Engine
{
    class RayCaster
    {
    public:
        RayCaster() = default;

        Game::HitRecord castRay(const Math::Ray &ray, const Game::Scene &scene,
                                float tMin, float tMax) const;

        void renderFrame(const Game::Scene &scene, Utils::Image &image) const;
    };
} // namespace Engine