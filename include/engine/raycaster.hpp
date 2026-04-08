#pragma once

#include "game/scene/scene.hpp"
#include "math/ray.hpp"
#include "utils/image.hpp"

namespace Engine
{
    /**
     * @brief A class for casting rays and performing ray tracing operations.
     *
     * The RayCaster class is responsible for casting rays into the scene and
     * determining what objects they intersect with.
     *
     * Engine classes execution flow:
     * - Runner updates its InputManager (updates the scene components fields)
     * - Runner updates its Scene (calculates physics, logic, etc.)
     * - RayCaster generates the current frame (based on the updated scene)
     * - Renderer takes the generated frame and updates the display
     */
    class RayCaster
    {
    public:
        RayCaster() = default;

        Game::HitRecord castRay(const Math::Ray &ray, const Game::Scene &scene,
                                float tMin, float tMax) const;

        void renderFrame(const Game::Scene &scene, Utils::Image &image) const;
    };
} // namespace Engine