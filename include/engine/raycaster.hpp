#pragma once

#include <vector>

#include "game/scene/scene.hpp"
#include "game/world/wall.hpp"
#include "math/ray.hpp"

namespace Engine
{
    /**
     * @brief Performs nearest-hit ray queries against scene walls.
     *
     * Renderer uses this class for each screen column to find the closest wall
     * intersection that will feed wall shading and projection.
     */
    class RayCaster
    {
    public:
        RayCaster() = default;

        std::vector<Game::HitRecord> castRay(const Math::Ray &ray, const Game::Scene &scene,
                                             float tMin, float tMax) const;
    };
} // namespace Engine
