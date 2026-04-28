#pragma once

#include <vector>

#include "game/scene/scene.hpp"
#include "game/world/wall.hpp"
#include "math/ray.hpp"

namespace Engine
{
    class RayCaster
    {
    public:
        RayCaster() = default;

        std::vector<Game::HitRecord> castRay(const Math::Ray &ray, const Game::Scene &scene,
                                             float tMin, float tMax) const;
    };
} // namespace Engine
